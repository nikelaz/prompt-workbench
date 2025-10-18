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

vm::user_prompts::UserPromptsViewModel vm::user_prompts::init(
    dba::DBAState& dba_state
)
{
    UserPromptsViewModel vm;
    return vm;
}

void vm::user_prompts::set_test_suite_id(
    dba::DBAState& dba_state,
    UserPromptsViewModel& user_prompts_vm,
    int64_t test_suite_id
)
{ 
    user_prompts_vm.current_test_suite_id = test_suite_id;
    user_prompts_vm.user_prompts = dba::get_all_user_prompts(dba_state, test_suite_id); 
    user_prompts_vm.result_runs = dba::get_all_result_runs(dba_state, test_suite_id);
}
