#include "dba.h"

namespace vm {
    struct TestSuitesViewModel
    {
        std::vector<TestSuite> test_suites;
    };

    void test_suites_vm_init(); 
    TestSuitesViewModel& test_suites_vm();
}
