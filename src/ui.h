#pragma once

#include "view-models.h"
#include "dba.h"

namespace ui {
    bool button(const std::string&); 
    void main_frame(
        vm::test_suites::TestSuitesViewModel& test_suites_vm,
        dba::DBAState& dba_state
    );
}
