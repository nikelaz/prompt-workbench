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

    namespace user_prompts {
        struct UserPromptsViewModel
        {
            std::vector<UserPrompt> user_prompts;
            std::vector<ResultRun> result_runs;
            std::optional<int64_t> current_test_suite_id;
            std::optional<UserPrompt> current_user_prompt;
        };

        UserPromptsViewModel init(dba::DBAState& dba_state);
        void set_test_suite_id(
            dba::DBAState& dba_state,
            UserPromptsViewModel& user_prompts_vm,
            int64_t test_suite_id
        );
    }
}
