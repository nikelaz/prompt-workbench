#pragma once

#include "dba.h"

namespace vm {
    namespace test_suites {
        struct TestSuitesViewModel
        {
            std::vector<TestSuite> test_suites;
            std::optional<TestSuite> current_test_suite;
        };

        TestSuitesViewModel init(dba::DBAState& dba_state); 
        void set_current_test_suite(
            TestSuitesViewModel& test_suites_vm,
            dba::DBAState& dba_state,
            int64_t id
        );
    }
}
