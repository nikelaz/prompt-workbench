#include "view-models.h"
#include "dba.h"

using std::optional;
using std::vector;

vm::TestSuitesViewModel& vm::test_suites_vm()
{
    static TestSuitesViewModel instance;
    return instance;
}

void vm::test_suites_vm_init() {
    optional<vector<TestSuite>> test_suites = dba::get_all_test_suites();    
    if (test_suites)
    {
        vm::test_suites_vm().test_suites = test_suites.value();
    }
}
