#include "view-models.h"

using std::vector;

vm::test_suites::TestSuitesViewModel vm::test_suites::init(
    dba::DBAState& dba_state
)
{
    TestSuitesViewModel ts_vm;
    vector<TestSuite> test_suites = dba::get_all_test_suites(dba_state);
    ts_vm.test_suites = test_suites;
    return ts_vm;
}
