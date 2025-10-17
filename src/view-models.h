#pragma once

#include "dba.h"

namespace vm {
    namespace test_suites {
        struct TestSuitesViewModel
        {
            std::vector<TestSuite> test_suites;
        };

        TestSuitesViewModel init(dba::DBAState& dba_state); 
    }
}
