#include "view-models.h"

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

vm::create_test_suite::CreateTestSuiteViewModel vm::create_test_suite::init()
{
    CreateTestSuiteViewModel vm;
    return vm;
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
