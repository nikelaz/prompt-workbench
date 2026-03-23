#include "embedding.h"
#include "llama.h"
#include <atomic>
#include <cmath>
#include <thread>
#include <iostream>

static llama_model* g_model = nullptr;
static std::atomic<bool> g_loading{false};
static std::atomic<bool> g_available{false};
static std::thread g_load_thread;

bool embedding::init(const std::string& model_path)
{
    g_loading.store(true);
    g_load_thread = std::thread([model_path]() {
        llama_backend_init();
        llama_model_params mparams = llama_model_default_params();
        mparams.n_gpu_layers = 0;
        g_model = llama_model_load_from_file(model_path.c_str(), mparams);
        if (!g_model)
            std::cerr << "[embedding] Failed to load model from: " << model_path << "\n";
        g_available.store(g_model != nullptr);
        g_loading.store(false);
    });
    return true;
}

void embedding::deinit()
{
    if (g_load_thread.joinable()) g_load_thread.join();
    if (g_model) {
        llama_model_free(g_model);
        g_model = nullptr;
    }
    llama_backend_free();
}

bool embedding::is_available()
{
    return g_available.load();
}

bool embedding::is_loading()
{
    return g_loading.load();
}

std::vector<float> embedding::compute(const std::string& text)
{
    if (!g_model || text.empty()) return {};

    llama_context_params cparams = llama_context_default_params();
    const int n_ctx = 8192;
    cparams.n_ctx         = n_ctx;
    cparams.n_batch       = n_ctx;
    cparams.n_ubatch      = n_ctx;
    cparams.embeddings    = true;
    cparams.pooling_type  = LLAMA_POOLING_TYPE_MEAN;
    cparams.n_threads     = static_cast<uint32_t>(std::thread::hardware_concurrency());

    // nomic-embed-text-v1.5 requires a task prefix; "clustering:" is correct
    // for symmetric document-to-document similarity comparison
    std::string prefixed = "clustering: " + text;

    // Tokenize into a buffer large enough for the full text
    const llama_vocab* vocab = llama_model_get_vocab(g_model);
    std::vector<llama_token> tokens(prefixed.size() + 16);
    int n = llama_tokenize(vocab, prefixed.c_str(), static_cast<int>(prefixed.size()),
                           tokens.data(), static_cast<int>(tokens.size()),
                           true, false);
    if (n < 0) return {};
    tokens.resize(n);

    int n_embd = llama_model_n_embd(g_model);

    auto embed_chunk = [&](llama_token* chunk_tokens, int chunk_len) -> std::vector<float> {
        llama_context* ctx = llama_init_from_model(g_model, cparams);
        if (!ctx) return {};

        llama_batch batch = llama_batch_get_one(chunk_tokens, chunk_len);
        if (llama_decode(ctx, batch) != 0) {
            llama_free(ctx);
            return {};
        }

        float* emb = llama_get_embeddings_seq(ctx, 0);
        if (!emb) {
            llama_free(ctx);
            return {};
        }

        std::vector<float> vec(emb, emb + n_embd);
        llama_free(ctx);
        return vec;
    };

    std::vector<float> result;

    if (n <= n_ctx) {
        result = embed_chunk(tokens.data(), n);
    } else {
        // Chunk + mean-pool for texts exceeding the context window
        std::vector<float> sum(n_embd, 0.0f);
        int chunk_count = 0;

        for (int start = 0; start < n; start += n_ctx) {
            int end = std::min(start + n_ctx, n);
            std::vector<float> chunk_emb = embed_chunk(tokens.data() + start, end - start);
            if (chunk_emb.empty()) return {};

            for (int i = 0; i < n_embd; ++i)
                sum[i] += chunk_emb[i];
            ++chunk_count;
        }

        result.resize(n_embd);
        for (int i = 0; i < n_embd; ++i)
            result[i] = sum[i] / static_cast<float>(chunk_count);
    }

    if (result.empty()) return {};

    // L2 normalize
    float norm = 0.0f;
    for (float v : result) norm += v * v;
    norm = std::sqrt(norm);
    if (norm > 0.0f)
        for (float& v : result) v /= norm;

    return result;
}
