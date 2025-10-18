#pragma once

#include "view-models.h"
#include "dba.h"
#include "routing.h"

namespace ui {
    namespace views {        
        void test_suites(
            routing::Router& router,
            dba::DBAState& dba_state,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::user_prompts::UserPromptsViewModel& user_prompts_vm
        );
        void test_suite_details(
            routing::Router& router,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::user_prompts::UserPromptsViewModel& user_prompts_vm
        );

        void user_prompt_details(
            routing::Router& router,
            vm::user_prompts::UserPromptsViewModel& user_prompts_vm
        );
    }

    namespace components {
        bool button(const std::string&); 
        void main_frame(
            dba::DBAState& dba_state,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::user_prompts::UserPromptsViewModel& user_prompts_vm
        );
        bool card(
            const std::string& id,
            const std::string& headline,
            const std::optional<std::string>& subtext,
            const std::optional<std::string>& right_label
        );
        void top_bar(routing::Router& router);
        void sidebar(std::function<void()> children);
        bool sidebar_button(
            const std::string& label,
            const bool is_active
        );
    }
}
