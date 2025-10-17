#include "view-models.h"

using std::vector;
using std::optional;

vm::test_suites::TestSuitesViewModel vm::test_suites::init(
    dba::DBAState& dba_state
)
{
    TestSuitesViewModel ts_vm;
    vector<TestSuite> test_suites = dba::get_all_test_suites(dba_state);
    ts_vm.test_suites = test_suites;
    return ts_vm;
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
