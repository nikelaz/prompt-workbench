#include "view-models.h"
#include "api.h"
#include <GLFW/glfw3.h>
#include <ctime>

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
        create_test_suite_vm.system_prompt,
        create_test_suite_vm.model
    );

    vm::test_suites::refresh(
        test_suites_vm,
        dba_state
    );
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
    create_test_suite_vm.system_prompt_error.has_error = false;
    create_test_suite_vm.system_prompt_error.message = "";
    create_test_suite_vm.model_error.has_error = false;
    create_test_suite_vm.model_error.message = "";

    if (!is_not_empty(create_test_suite_vm.title))
    {
        create_test_suite_vm.title_error.has_error = true;
        create_test_suite_vm.title_error.message =
            "Title is required";
    }

    if (!has_max_length(
        create_test_suite_vm.title,
        255
    ))  
    {
        create_test_suite_vm.title_error.has_error = true;
        create_test_suite_vm.title_error.message = 
            "Title has to be less than 255 characters";
    }

    if (!is_not_empty(create_test_suite_vm.description))
    {
        create_test_suite_vm.description_error.has_error = true;
        create_test_suite_vm.description_error.message =
            "Description is required";
    }

    if (!has_max_length(
        create_test_suite_vm.description,
        1000
    ))  
    {
        create_test_suite_vm.description_error.has_error = true;
        create_test_suite_vm.description_error.message = 
            "Description has to be less than 1000 characters";
    }

    if (!is_not_empty(create_test_suite_vm.system_prompt))
    {
        create_test_suite_vm.system_prompt_error.has_error = true;
        create_test_suite_vm.system_prompt_error.message =
            "System prompt is required";
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

void vm::edit_test_suite::prepare(
    EditTestSuiteViewModel& vm,
    const TestSuite& suite
)
{
    vm.id = suite.id;
    vm.title = suite.title;
    vm.description = suite.description;
    vm.system_prompt = suite.system_prompt;
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
        vm.system_prompt,
        vm.model
    );

    vm::test_suites::refresh(test_suites_vm, dba_state);
}

void vm::edit_test_suite::validate(EditTestSuiteViewModel& vm)
{
    vm.title_error.has_error = false;
    vm.title_error.message = "";
    vm.description_error.has_error = false;
    vm.description_error.message = "";
    vm.system_prompt_error.has_error = false;
    vm.system_prompt_error.message = "";
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

    if (!is_not_empty(vm.description))
    {
        vm.description_error.has_error = true;
        vm.description_error.message = "Description is required";
    }

    if (!has_max_length(vm.description, 1000))
    {
        vm.description_error.has_error = true;
        vm.description_error.message = "Description has to be less than 1000 characters";
    }

    if (!is_not_empty(vm.system_prompt))
    {
        vm.system_prompt_error.has_error = true;
        vm.system_prompt_error.message = "System prompt is required";
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
    const Settings& settings
)
{
    if (vm.is_running.load()) return;

    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
    vm.run_date = buf;

    vm.pending_results.clear();
    vm.is_running.store(true);

    std::thread([&vm, suite, prompts, settings]() {
        std::vector<RunResult> local_results;
        for (const auto& prompt : prompts) {
            std::string answer = api::openai_ask(
                settings.api_endpoint,
                settings.api_key,
                suite.model,
                suite.system_prompt,
                prompt.prompt
            );
            local_results.push_back({ prompt.id, answer });
        }
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
        user_prompt_vm.current_test_suite_id.value()
    );

    if (!run_id) {
        vm.pending_results.clear();
        return;
    }

    for (const auto& r : vm.pending_results) {
        dba::create_answer(dba_state, r.answer, r.user_prompt_id, *run_id);
    }

    vm.pending_results.clear();
    vm::user_prompt::refresh(user_prompt_vm, dba_state);
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
