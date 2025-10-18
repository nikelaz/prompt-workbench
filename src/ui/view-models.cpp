#include "view-models.h"

using std::vector;
using std::optional;

vm::test_suites::TestSuitesViewModel vm::test_suites::init(
    dba::DBAState& dba_state
)
{
    TestSuitesViewModel vm;
    vector<TestSuite> test_suites = dba::get_all_test_suites(dba_state);
    vm.test_suites = test_suites;
    return vm;
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

vm::user_prompt_details::UserPromptDetailsViewModel vm::user_prompt_details::init(
    dba::DBAState& dba_state
)
{
    UserPromptDetailsViewModel vm;
    return vm;
}

void vm::user_prompt_details::set_test_suite_id(
    dba::DBAState& dba_state,
    UserPromptDetailsViewModel& user_prompts_vm,
    int64_t test_suite_id
)
{ 
    user_prompts_vm.current_test_suite_id = test_suite_id;
    user_prompts_vm.user_prompts = dba::get_all_user_prompts(dba_state, test_suite_id); 
    user_prompts_vm.result_runs = dba::get_all_result_runs(dba_state, test_suite_id);
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
