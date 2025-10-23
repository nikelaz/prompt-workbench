#pragma once

#include "view-models.h"
#include "dba.h"
#include "routing.h"
#include "fonts.h"
#include "errors.h"

namespace ui {
    namespace views {        
        void test_suites(
            routing::Router& router,
            dba::DBAState& dba_state,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::user_prompt::UserPromptViewModel& user_prompt_details_vm
        );
        void test_suite_details(
            routing::Router& router,
            dba::DBAState& dba_state,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::user_prompt::UserPromptViewModel& user_prompt_details_vm,
            vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm
        );
        void create_test_suite(
            routing::Router& router,
            dba::DBAState& dba_state,
            fonts::Fonts& fonts,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::create_test_suite::CreateTestSuiteViewModel& create_test_suite_vm
        );
        void user_prompt_details(
            routing::Router& router,
            vm::user_prompt::UserPromptViewModel& user_prompt_details_vm
        );
        void create_user_prompts(
            routing::Router& router,
            dba::DBAState& dba_state,
            fonts::Fonts& fonts,
            vm::create_user_prompt::CreateUserPromptViewModel& test_suites_vm,
            vm::user_prompt::UserPromptViewModel& user_prompts_vm
        );
        void result_run_details(
            routing::Router& router,
            vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm
        );

    }

    namespace components {
        bool button(const std::string&); 
        void main_frame(
            dba::DBAState& dba_state,
            fonts::Fonts& fonts,
            vm::test_suites::TestSuitesViewModel& test_suites_vm,
            vm::user_prompt::UserPromptViewModel& user_prompt_details_vm,
            vm::result_run_details::ResultRunDetailsViewModel& result_run_details_vm,
            vm::create_test_suite::CreateTestSuiteViewModel& create_test_suite_vm,
            vm::create_user_prompt::CreateUserPromptViewModel& create_user_prompt_vm 
        );
        bool input(
            const char* label,
            std::string* str
        );
        bool input_multiline(
            const char* label,
            std::string* str
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
        void input_error(
            const errors::DisplayError& error,
            const fonts::Fonts& fonts
        ); 
    }
}
