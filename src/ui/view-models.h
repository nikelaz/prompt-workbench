#pragma once

#include "dba.h"

namespace vm
{
    namespace test_suites
    {
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

    namespace user_prompt_details {
        struct UserPromptDetailsViewModel
        {
            std::vector<UserPrompt> user_prompts;
            std::vector<ResultRun> result_runs;
            std::optional<int64_t> current_test_suite_id;
            std::optional<UserPrompt> current_user_prompt;
        };

        UserPromptDetailsViewModel init(dba::DBAState& dba_state);
        void set_test_suite_id(
            dba::DBAState& dba_state,
            UserPromptDetailsViewModel& user_prompts_vm,
            int64_t test_suite_id
        );
    }

    namespace result_run_details {
        struct ResultRunDetailsViewModel
        {
            std::vector<Answer> answers;
            std::optional<ResultRun> current_result_run;
        };

        ResultRunDetailsViewModel init(dba::DBAState& dba_state);
        void set_current_result_run(
            dba::DBAState& dba_state,
            ResultRunDetailsViewModel& result_run_details_vm,
            ResultRun current_result_run
        );
    }
}
