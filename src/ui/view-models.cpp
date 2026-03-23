#include "view-models.h"
#include "api.h"
#include "helpers.h"
#include "embedding.h"
#include <GLFW/glfw3.h>
#include <ctime>
#include <numeric>
#include <fstream>
#include <chrono>

using std::vector;
using std::optional;

vm::test_suites::TestSuitesViewModel vm::test_suites::init(
    dba::DBAState& dba_state
)
{
    TestSuitesViewModel vm;
    refresh(vm, dba_state);
    return vm;
}

void vm::test_suites::refresh(
    TestSuitesViewModel& test_suites_vm,
    dba::DBAState& dba_state
)
{
    vector<TestSuite> test_suites =
        dba::get_all_test_suites(dba_state);
    test_suites_vm.test_suites = test_suites;
}

void vm::test_suites::refresh_for_prompt(
    TestSuitesViewModel& test_suites_vm,
    dba::DBAState& dba_state,
    int64_t prompt_id
)
{
    test_suites_vm.test_suites = dba::get_test_suites_for_prompt(dba_state, prompt_id);
    test_suites_vm.current_prompt_id = prompt_id;
}

void vm::test_suites::set_current_test_suite(
    TestSuitesViewModel& test_suites_vm,
    dba::DBAState& dba_state,
    int64_t id
)
{
    optional<TestSuite> test_suite = dba::get_test_suite(dba_state, id); 
    if (test_suite)
    {
        test_suites_vm.current_test_suite = test_suite.value();
    }
}

vm::user_prompt::UserPromptViewModel vm::user_prompt::init(
    dba::DBAState& dba_state
)
{
    UserPromptViewModel vm;
    return vm;
}

void vm::user_prompt::set_test_suite_id(
    dba::DBAState& dba_state,
    UserPromptViewModel& user_prompt_vm,
    int64_t test_suite_id
)
{ 
    user_prompt_vm.current_test_suite_id = test_suite_id;
    refresh(user_prompt_vm, dba_state);
}


void vm::user_prompt::refresh(
    UserPromptViewModel& user_prompt_vm,
    dba::DBAState& dba_state
)
{
    if (!user_prompt_vm.current_test_suite_id)
    {
        return;
    }

    user_prompt_vm.user_prompts = 
        dba::get_all_user_prompts(
            dba_state,
            user_prompt_vm.current_test_suite_id.value()
        ); 

    user_prompt_vm.result_runs = 
        dba::get_all_result_runs(
            dba_state, 
            user_prompt_vm.current_test_suite_id.value()
        );
}

vm::result_run_details::ResultRunDetailsViewModel vm::result_run_details::init(
    dba::DBAState& dba_state
)
{
    ResultRunDetailsViewModel vm;
    return vm;
}

// Todo: here we copy current_result_run (X times),
// check if I can move value instead for efficiency.
void vm::result_run_details::set_current_result_run(
    dba::DBAState& dba_state,
    ResultRunDetailsViewModel& result_run_details_vm,
    ResultRun current_result_run
)
{
    result_run_details_vm.current_result_run = current_result_run;
    result_run_details_vm.answers = dba::get_all_answers(dba_state, current_result_run.id);
}

void vm::create_test_suite::create_test_suite(
    dba::DBAState& dba_state,
    CreateTestSuiteViewModel& create_test_suite_vm,
    test_suites::TestSuitesViewModel& test_suites_vm
)
{
    optional<int64_t> test_suite_id = dba::create_test_suite(
        dba_state,
        create_test_suite_vm.title,
        create_test_suite_vm.description,
        "",
        create_test_suite_vm.model,
        test_suites_vm.current_prompt_id
    );

    if (test_suites_vm.current_prompt_id) {
        vm::test_suites::refresh_for_prompt(test_suites_vm, dba_state, *test_suites_vm.current_prompt_id);
    } else {
        vm::test_suites::refresh(test_suites_vm, dba_state);
    }
}

bool is_not_empty(const std::string& s)
{
    return !s.empty();
}

bool has_max_length(
    const std::string& s,
    const int length
)
{
    return s.length() <= length;
}

void vm::create_test_suite::validate(
    CreateTestSuiteViewModel& create_test_suite_vm
)
{
    create_test_suite_vm.title_error.has_error = false;
    create_test_suite_vm.title_error.message = "";
    create_test_suite_vm.description_error.has_error = false;
    create_test_suite_vm.description_error.message = "";
    create_test_suite_vm.model_error.has_error = false;
    create_test_suite_vm.model_error.message = "";

    if (!is_not_empty(create_test_suite_vm.title))
    {
        create_test_suite_vm.title_error.has_error = true;
        create_test_suite_vm.title_error.message =
            "Title is required";
    }

    if (!has_max_length(create_test_suite_vm.title, 255))
    {
        create_test_suite_vm.title_error.has_error = true;
        create_test_suite_vm.title_error.message =
            "Title has to be less than 255 characters";
    }

    if (!has_max_length(create_test_suite_vm.description, 1000))
    {
        create_test_suite_vm.description_error.has_error = true;
        create_test_suite_vm.description_error.message =
            "Description has to be less than 1000 characters";
    }

    if (!is_not_empty(create_test_suite_vm.model))
    {
        create_test_suite_vm.model_error.has_error = true;
        create_test_suite_vm.model_error.message =
            "Model is required";
    }

    if (!has_max_length(create_test_suite_vm.model, 255))
    {
        create_test_suite_vm.model_error.has_error = true;
        create_test_suite_vm.model_error.message =
            "Model has to be less than 255 characters";
    }
}

void vm::create_test_suite::fetch_models(
    CreateTestSuiteViewModel& vm,
    const Settings& settings
)
{
    if (vm.models_loading.load()) return;
    vm.models_loading.store(true);

    std::thread([&vm, settings]() {
        std::vector<std::string> models = api::get_models(
            settings.api_endpoint,
            settings.api_key
        );
        vm.available_models = std::move(models);
        vm.models_loaded = true;
        vm.models_loading.store(false);
        glfwPostEmptyEvent();
    }).detach();
}

void vm::prompt_editor::fetch_chat_models(
    PromptEditorViewModel& vm,
    const Settings& settings
)
{
    if (vm.chat_models_loading.load()) return;
    vm.chat_models_loading.store(true);

    std::thread([&vm, settings]() {
        vm.chat_available_models = api::get_models(settings.api_endpoint, settings.api_key);
        vm.chat_models_loaded = true;
        vm.chat_models_loading.store(false);
        glfwPostEmptyEvent();
    }).detach();
}

void vm::edit_test_suite::prepare(
    EditTestSuiteViewModel& vm,
    const TestSuite& suite
)
{
    vm.id = suite.id;
    vm.title = suite.title;
    vm.description = suite.description;
    vm.model = suite.model;
    vm.models_loaded = false;
    vm.models_loading.store(false);
    vm.available_models.clear();
    vm.model_selection = 0;
}

void vm::edit_test_suite::update_test_suite(
    dba::DBAState& dba_state,
    EditTestSuiteViewModel& vm,
    test_suites::TestSuitesViewModel& test_suites_vm
)
{
    dba::update_test_suite(
        dba_state,
        vm.id,
        vm.title,
        vm.description,
        std::nullopt,
        vm.model
    );

    if (test_suites_vm.current_prompt_id) {
        vm::test_suites::refresh_for_prompt(test_suites_vm, dba_state, *test_suites_vm.current_prompt_id);
    } else {
        vm::test_suites::refresh(test_suites_vm, dba_state);
    }
}

void vm::edit_test_suite::validate(EditTestSuiteViewModel& vm)
{
    vm.title_error.has_error = false;
    vm.title_error.message = "";
    vm.description_error.has_error = false;
    vm.description_error.message = "";
    vm.model_error.has_error = false;
    vm.model_error.message = "";

    if (!is_not_empty(vm.title))
    {
        vm.title_error.has_error = true;
        vm.title_error.message = "Title is required";
    }

    if (!has_max_length(vm.title, 255))
    {
        vm.title_error.has_error = true;
        vm.title_error.message = "Title has to be less than 255 characters";
    }

    if (!has_max_length(vm.description, 1000))
    {
        vm.description_error.has_error = true;
        vm.description_error.message = "Description has to be less than 1000 characters";
    }

    if (!is_not_empty(vm.model))
    {
        vm.model_error.has_error = true;
        vm.model_error.message = "Model is required";
    }

    if (!has_max_length(vm.model, 255))
    {
        vm.model_error.has_error = true;
        vm.model_error.message = "Model has to be less than 255 characters";
    }
}

void vm::edit_test_suite::fetch_models(
    EditTestSuiteViewModel& vm,
    const Settings& settings
)
{
    if (vm.models_loading.load()) return;
    vm.models_loading.store(true);

    std::string current_model = vm.model;
    std::thread([&vm, settings, current_model]() {
        std::vector<std::string> models = api::get_models(
            settings.api_endpoint,
            settings.api_key
        );
        int selection = 0;
        for (int i = 0; i < (int)models.size(); i++) {
            if (models[i] == current_model) { selection = i; break; }
        }
        vm.available_models = std::move(models);
        vm.model_selection = selection;
        vm.models_loaded = true;
        vm.models_loading.store(false);
        glfwPostEmptyEvent();
    }).detach();
}

vm::api_credentials::ApiCredentialsViewModel vm::api_credentials::init(
    dba::DBAState& dba_state
)
{
    static const char* preset_urls[] = {
        "https://api.openai.com/v1/chat/completions",
        "https://openrouter.ai/api/v1/chat/completions",
        "https://api.groq.com/openai/v1/chat/completions",
        "http://localhost:11434/v1/chat/completions",
    };
    static const int preset_count = 4;

    ApiCredentialsViewModel vm;
    Settings settings = dba::get_settings(dba_state);
    vm.api_endpoint = settings.api_endpoint;
    vm.api_key = settings.api_key;

    vm.endpoint_selection = preset_count; // default to Custom
    for (int i = 0; i < preset_count; i++) {
        if (settings.api_endpoint == preset_urls[i]) {
            vm.endpoint_selection = i;
            break;
        }
    }
    if (vm.endpoint_selection == preset_count) {
        vm.custom_endpoint = settings.api_endpoint;
    }

    return vm;
}

void vm::api_credentials::validate(ApiCredentialsViewModel& vm)
{
    vm.api_endpoint_error.has_error = false;
    vm.api_endpoint_error.message = "";
    vm.api_key_error.has_error = false;
    vm.api_key_error.message = "";

    if (!is_not_empty(vm.api_endpoint))
    {
        vm.api_endpoint_error.has_error = true;
        vm.api_endpoint_error.message = "API endpoint is required";
    }

    if (!has_max_length(vm.api_endpoint, 2048))
    {
        vm.api_endpoint_error.has_error = true;
        vm.api_endpoint_error.message = "API endpoint has to be less than 2048 characters";
    }

    if (!is_not_empty(vm.api_key))
    {
        vm.api_key_error.has_error = true;
        vm.api_key_error.message = "API key is required";
    }

    if (!has_max_length(vm.api_key, 512))
    {
        vm.api_key_error.has_error = true;
        vm.api_key_error.message = "API key has to be less than 512 characters";
    }
}

void vm::run_tests::run(
    RunTestsViewModel& vm,
    const TestSuite& suite,
    const std::vector<UserPrompt>& prompts,
    const Settings& settings,
    const std::string& system_prompt
)
{
    if (vm.is_running.load()) return;

    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    vm.run_date = buf;
    vm.system_prompt = system_prompt;

    vm.pending_results.clear();
    vm.abort_requested.store(false);
    vm.completed_count.store(0);
    vm.total_count = static_cast<int>(prompts.size());
    vm.is_running.store(true);

    std::string captured_system_prompt = system_prompt;
    std::thread([&vm, suite, prompts, settings, captured_system_prompt]() {
        std::vector<RunResult> local_results;
        for (const auto& prompt : prompts) {
            if (vm.abort_requested.load()) break;
            std::string answer = api::openai_ask(
                settings.api_endpoint,
                settings.api_key,
                suite.model,
                captured_system_prompt,
                prompt.prompt
            );
            std::vector<float> emb = embedding::compute(answer);
            local_results.push_back({ prompt.id, answer, std::move(emb) });
            vm.completed_count.fetch_add(1);
            glfwPostEmptyEvent();
        }
        if (!vm.abort_requested.load())
            vm.pending_results = std::move(local_results);
        vm.is_running.store(false);
        glfwPostEmptyEvent();
    }).detach();
}

void vm::run_tests::commit_if_done(
    RunTestsViewModel& vm,
    dba::DBAState& dba_state,
    vm::user_prompt::UserPromptViewModel& user_prompt_vm
)
{
    if (vm.is_running.load()) return;
    if (vm.pending_results.empty()) return;

    if (!user_prompt_vm.current_test_suite_id) {
        vm.pending_results.clear();
        return;
    }

    optional<int64_t> run_id = dba::create_result_run(
        dba_state,
        vm.run_date,
        user_prompt_vm.current_test_suite_id.value(),
        vm.system_prompt
    );

    if (!run_id) {
        vm.pending_results.clear();
        return;
    }

    for (const auto& r : vm.pending_results) {
        auto answer_id = dba::create_answer(dba_state, r.answer, r.user_prompt_id, *run_id);
        if (answer_id && !r.embedding.empty())
            dba::store_embedding(dba_state, *answer_id, r.embedding);
    }

    vm.pending_results.clear();
    vm::user_prompt::refresh(user_prompt_vm, dba_state);
}

vm::compare_result_runs::CompareResultRunsViewModel vm::compare_result_runs::init()
{
    return CompareResultRunsViewModel{};
}

void vm::compare_result_runs::prepare(
    dba::DBAState& dba_state,
    CompareResultRunsViewModel& vm,
    const ResultRun& run_a,
    const ResultRun& run_b,
    const std::vector<UserPrompt>& user_prompts
)
{
    vm.run_a = run_a;
    vm.run_b = run_b;
    vm.user_prompts = user_prompts;
    vm.answers_a = dba::get_all_answers(dba_state, run_a.id);
    vm.answers_b = dba::get_all_answers(dba_state, run_b.id);
    vm.diff_scores.clear();
    compute_similarities(vm);
}

void vm::compare_result_runs::compute_similarities(CompareResultRunsViewModel& vm)
{
    const size_t n = vm.user_prompts.size();
    std::vector<float> local_scores(n, -1.0f);

    for (size_t i = 0; i < n; i++) {
        int64_t up_id = vm.user_prompts[i].id;

        const std::vector<float>* emb_a = nullptr;
        const std::vector<float>* emb_b = nullptr;
        for (const Answer& a : vm.answers_a)
            if (a.user_prompt_id == up_id) { emb_a = &a.embedding; break; }
        for (const Answer& a : vm.answers_b)
            if (a.user_prompt_id == up_id) { emb_b = &a.embedding; break; }

        if (emb_a && emb_b && !emb_a->empty() && !emb_b->empty()) {
            float sim = helpers::cosine_similarity(*emb_a, *emb_b);
            local_scores[i] = (1.0f - sim) * 100.0f;
        }
    }

    // Sort: valid scores descending, invalid (-1) at end
    std::vector<size_t> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&](size_t a, size_t b) {
        if (local_scores[a] < 0.0f && local_scores[b] < 0.0f) return false;
        if (local_scores[a] < 0.0f) return false;
        if (local_scores[b] < 0.0f) return true;
        return local_scores[a] > local_scores[b];
    });

    std::vector<UserPrompt> sorted_prompts(n);
    std::vector<Answer>     sorted_a(n), sorted_b(n);
    std::vector<float>      sorted_scores(n);
    for (size_t i = 0; i < n; i++) {
        sorted_prompts[i] = vm.user_prompts[idx[i]];
        sorted_scores[i]  = local_scores[idx[i]];

        for (const Answer& a : vm.answers_a)
            if (a.user_prompt_id == vm.user_prompts[idx[i]].id) { sorted_a[i] = a; break; }
        for (const Answer& a : vm.answers_b)
            if (a.user_prompt_id == vm.user_prompts[idx[i]].id) { sorted_b[i] = a; break; }
    }

    vm.user_prompts = std::move(sorted_prompts);
    vm.answers_a    = std::move(sorted_a);
    vm.answers_b    = std::move(sorted_b);
    vm.diff_scores  = std::move(sorted_scores);
}

vm::create_user_prompt::CreateUserPromptViewModel vm::create_user_prompt::init()
{
    CreateUserPromptViewModel vm;
    return vm;
}

void vm::create_user_prompt::create_user_prompt(
    dba::DBAState& dba_state,
    CreateUserPromptViewModel& create_user_prompt_vm,
    user_prompt::UserPromptViewModel& user_prompt_vm
)
{
    if (!user_prompt_vm.current_test_suite_id)
    {
        return;
    }

    optional<int64_t> user_prompt_id = dba::create_user_prompt(
        dba_state,
        create_user_prompt_vm.prompt,
        user_prompt_vm.current_test_suite_id.value()
    );
}

void vm::create_user_prompt::validate(
    CreateUserPromptViewModel& create_user_prompt_vm
)
{
    create_user_prompt_vm.prompt_error.has_error = false;
    create_user_prompt_vm.prompt_error.message = "";

    if (!is_not_empty(create_user_prompt_vm.prompt))
    {
        create_user_prompt_vm.prompt_error.has_error = true;
        create_user_prompt_vm.prompt_error.message =
            "Prompt is required";
    }
}

void vm::edit_user_prompt::prepare(
    EditUserPromptViewModel& vm,
    const UserPrompt& user_prompt
)
{
    vm.id = user_prompt.id;
    vm.prompt = user_prompt.prompt;
    vm.prompt_error.has_error = false;
    vm.prompt_error.message = "";
}

void vm::edit_user_prompt::update_user_prompt(
    dba::DBAState& dba_state,
    EditUserPromptViewModel& vm,
    user_prompt::UserPromptViewModel& user_prompt_vm
)
{
    dba::update_user_prompt(
        dba_state,
        vm.id,
        optional<std::string>(vm.prompt),
        std::nullopt
    );

    vm::user_prompt::refresh(user_prompt_vm, dba_state);
}

void vm::edit_user_prompt::validate(EditUserPromptViewModel& vm)
{
    vm.prompt_error.has_error = false;
    vm.prompt_error.message = "";

    if (!is_not_empty(vm.prompt))
    {
        vm.prompt_error.has_error = true;
        vm.prompt_error.message = "Prompt is required";
    }
}

vm::prompt_editor::PromptEditorViewModel vm::prompt_editor::init(dba::DBAState& dba_state)
{
    PromptEditorViewModel vm;
    vm.open_prompts = dba::get_all_open_prompts(dba_state);
    if (!vm.open_prompts.empty()) {
        std::ifstream f(vm.open_prompts[0].file_path);
        if (f.good()) {
            vm.active_file_content = std::string(
                std::istreambuf_iterator<char>(f),
                std::istreambuf_iterator<char>()
            );
        }
        vm.active_prompt_id = vm.open_prompts[0].id;
    }
    return vm;
}

void vm::prompt_editor::open_file(
    PromptEditorViewModel& vm,
    dba::DBAState& dba_state,
    const std::string& path
)
{
    auto id = dba::open_prompt(dba_state, path);
    if (!id) return;

    vm.open_prompts = dba::get_all_open_prompts(dba_state);

    std::ifstream f(path);
    vm.active_file_content = f.good()
        ? std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>())
        : "";
    vm.active_prompt_id = *id;
    vm.content_dirty = false;
    vm.active_tab = 0;
}

void vm::prompt_editor::set_active_prompt(
    PromptEditorViewModel& vm,
    dba::DBAState& dba_state,
    int64_t id
)
{
    for (auto& p : vm.open_prompts) {
        if (p.id == id) {
            vm.active_prompt_id = id;
            std::ifstream f(p.file_path);
            vm.active_file_content = f.good()
                ? std::string(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>())
                : "";
            vm.content_dirty = false;
            vm.active_tab = 0;
            return;
        }
    }
}

void vm::prompt_editor::close_prompt(
    PromptEditorViewModel& vm,
    dba::DBAState& dba_state,
    int64_t id
)
{
    save_if_dirty(vm);
    dba::close_prompt(dba_state, id);
    vm.open_prompts = dba::get_all_open_prompts(dba_state);

    if (vm.active_prompt_id == id) {
        if (!vm.open_prompts.empty()) {
            set_active_prompt(vm, dba_state, vm.open_prompts[0].id);
        } else {
            vm.active_prompt_id = std::nullopt;
            vm.active_file_content = "";
        }
    }
}

void vm::prompt_editor::save_if_dirty(PromptEditorViewModel& vm)
{
    if (!vm.content_dirty) return;
    if (!vm.active_prompt_id) return;

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - vm.last_edit_time);
    if (elapsed.count() < 500) return;

    for (auto& p : vm.open_prompts) {
        if (p.id == *vm.active_prompt_id) {
            std::ofstream f(p.file_path, std::ios::trunc);
            if (f.good()) {
                f << vm.active_file_content;
                vm.content_dirty = false;
            }
            break;
        }
    }
}

void vm::prompt_editor::send_chat_message(
    PromptEditorViewModel& vm,
    const Settings& settings
)
{
    if (vm.chat_is_sending.load()) return;
    if (vm.chat_input.empty()) return;

    vm.chat_history.push_back({ "user", vm.chat_input });
    std::string user_msg = vm.chat_input;
    vm.chat_input = "";
    vm.chat_is_sending.store(true);

    std::string system_prompt = vm.active_file_content;
    std::string model = vm.chat_model;

    std::thread([&vm, settings, system_prompt, model, user_msg]() {
        std::string response = api::openai_ask(
            settings.api_endpoint,
            settings.api_key,
            model,
            system_prompt,
            user_msg
        );
        vm.chat_pending_response = std::move(response);
        vm.chat_is_sending.store(false);
        glfwPostEmptyEvent();
    }).detach();
}

void vm::prompt_editor::commit_chat_if_done(PromptEditorViewModel& vm)
{
    if (vm.chat_is_sending.load()) return;
    if (vm.chat_pending_response.empty()) return;

    vm.chat_history.push_back({ "assistant", std::move(vm.chat_pending_response) });
    vm.chat_pending_response = "";
}
