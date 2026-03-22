#include "embedding.h"
#include "llama.h"
#include <cmath>
#include <thread>
#include <iostream>

static llama_model* g_model = nullptr;

bool embedding::init(const std::string& model_path)
{
    llama_backend_init();

    llama_model_params mparams = llama_model_default_params();
    mparams.n_gpu_layers = 0;

    g_model = llama_model_load_from_file(model_path.c_str(), mparams);
    if (!g_model) {
        std::cerr << "[embedding] Failed to load model from: " << model_path << std::endl;
        return false;
    }

    return true;
}

void embedding::deinit()
{
    if (g_model) {
        llama_model_free(g_model);
        g_model = nullptr;
    }
    llama_backend_free();
}

bool embedding::is_available()
{
    return g_model != nullptr;
}

std::vector<float> embedding::compute(const std::string& text)
{
    if (!g_model || text.empty()) return {};

    llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx         = 8192;
    cparams.n_batch       = 8192;
    cparams.embeddings    = true;
    cparams.pooling_type  = LLAMA_POOLING_TYPE_MEAN;
    cparams.n_threads     = static_cast<uint32_t>(std::thread::hardware_concurrency());

    llama_context* ctx = llama_init_from_model(g_model, cparams);
    if (!ctx) return {};

    // nomic-embed-text-v1.5 requires a task prefix; "clustering:" is correct
    // for symmetric document-to-document similarity comparison
    std::string prefixed = "clustering: " + text;

    // Tokenize
    const llama_vocab* vocab = llama_model_get_vocab(g_model);
    std::vector<llama_token> tokens(8192);
    int n = llama_tokenize(vocab, prefixed.c_str(), static_cast<int>(prefixed.size()),
                           tokens.data(), static_cast<int>(tokens.size()),
                           true, false);
    if (n < 0) {
        llama_free(ctx);
        return {};
    }
    tokens.resize(n);

    // Run forward pass
    llama_batch batch = llama_batch_get_one(tokens.data(), static_cast<int>(tokens.size()));
    if (llama_decode(ctx, batch) != 0) {
        llama_free(ctx);
        return {};
    }

    // Get pooled embedding for sequence 0
    float* emb = llama_get_embeddings_seq(ctx, 0);
    if (!emb) {
        llama_free(ctx);
        return {};
    }

    int n_embd = llama_model_n_embd(g_model);
    std::vector<float> result(emb, emb + n_embd);

    // L2 normalize
    float norm = 0.0f;
    for (float v : result) norm += v * v;
    norm = std::sqrt(norm);
    if (norm > 0.0f)
        for (float& v : result) v /= norm;

    llama_free(ctx);
    return result;
}
