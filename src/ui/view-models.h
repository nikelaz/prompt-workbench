#pragma once

#include "dba.h"
#include "errors.h"
#include <atomic>
#include <thread>

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
        void refresh(
            TestSuitesViewModel& test_suites_vm,
            dba::DBAState& dba_state
        );      
    }

    namespace user_prompt {
        struct UserPromptViewModel
        {
            std::vector<UserPrompt> user_prompts;
            std::vector<ResultRun> result_runs;
            std::optional<int64_t> current_test_suite_id;
            std::optional<UserPrompt> current_user_prompt;
        };

        UserPromptViewModel init(dba::DBAState& dba_state);
        void set_test_suite_id(
            dba::DBAState& dba_state,
            UserPromptViewModel& user_prompts_vm,
            int64_t test_suite_id
        ); 
        void refresh(
            UserPromptViewModel& user_prompt_vm,
            dba::DBAState& dba_state
        ); 
    }

    namespace result_run_details {
        struct ResultRunDetailsViewModel
        {
            std::vector<Answer> answers;
            std::optional<ResultRun> current_result_run;
            std::optional<Answer> current_answer;
            std::optional<UserPrompt> current_user_prompt;
            bool editing_title = false;
            std::string title_edit_buffer = "";
        };

        ResultRunDetailsViewModel init(dba::DBAState& dba_state);
        void set_current_result_run(
            dba::DBAState& dba_state,
            ResultRunDetailsViewModel& result_run_details_vm,
            ResultRun current_result_run
        );
    }

    namespace create_test_suite {
        struct CreateTestSuiteViewModel
        {
            std::string title = "";
            errors::DisplayError title_error;
            std::string description = "";
            errors::DisplayError description_error;
            std::string system_prompt = "";
            errors::DisplayError system_prompt_error;
            std::string model = "";
            errors::DisplayError model_error;
            std::vector<std::string> available_models;
            std::atomic<bool> models_loading{false};
            bool models_loaded = false;
            int model_selection = 0;
        };

        void create_test_suite(
            dba::DBAState& dba_state,
            CreateTestSuiteViewModel& create_test_suite_vm,
            test_suites::TestSuitesViewModel& test_suites_vm
        );
        void validate(
            CreateTestSuiteViewModel& create_test_suite_vm
        );
        void fetch_models(
            CreateTestSuiteViewModel& vm,
            const Settings& settings
        );
    }

    namespace edit_test_suite {
        struct EditTestSuiteViewModel
        {
            int64_t id = 0;
            std::string title = "";
            errors::DisplayError title_error;
            std::string description = "";
            errors::DisplayError description_error;
            std::string system_prompt = "";
            errors::DisplayError system_prompt_error;
            std::string model = "";
            errors::DisplayError model_error;
            std::vector<std::string> available_models;
            std::atomic<bool> models_loading{false};
            bool models_loaded = false;
            int model_selection = 0;
        };

        void prepare(EditTestSuiteViewModel& vm, const TestSuite& suite);
        void update_test_suite(
            dba::DBAState& dba_state,
            EditTestSuiteViewModel& vm,
            test_suites::TestSuitesViewModel& test_suites_vm
        );
        void validate(EditTestSuiteViewModel& vm);
        void fetch_models(EditTestSuiteViewModel& vm, const Settings& settings);
    }

    namespace api_credentials {
        struct ApiCredentialsViewModel
        {
            int endpoint_selection = 0;
            std::string custom_endpoint = "";
            std::string api_endpoint = "";
            errors::DisplayError api_endpoint_error;
            std::string api_key = "";
            errors::DisplayError api_key_error;
            bool saved = false;
        };

        ApiCredentialsViewModel init(dba::DBAState& state);
        void validate(ApiCredentialsViewModel& vm);
    }

    namespace run_tests {
        struct RunResult {
            int64_t user_prompt_id;
            std::string answer;
            std::vector<float> embedding;
        };

        struct RunTestsViewModel {
            std::atomic<bool> is_running{false};
            std::atomic<bool> abort_requested{false};
            std::atomic<int> completed_count{0};
            int total_count = 0;
            std::vector<RunResult> pending_results;
            std::string run_date;
            std::string system_prompt;
        };

        void run(
            RunTestsViewModel& vm,
            const TestSuite& suite,
            const std::vector<UserPrompt>& prompts,
            const Settings& settings
        );

        void commit_if_done(
            RunTestsViewModel& vm,
            dba::DBAState& dba_state,
            user_prompt::UserPromptViewModel& user_prompt_vm
        );
    }

    namespace compare_result_runs {
        struct CompareResultRunsViewModel {
            ResultRun run_a;
            ResultRun run_b;
            std::vector<Answer> answers_a;
            std::vector<Answer> answers_b;
            std::vector<UserPrompt> user_prompts;
            std::vector<float> diff_scores;
        };

        CompareResultRunsViewModel init();
        void prepare(
            dba::DBAState& dba_state,
            CompareResultRunsViewModel& vm,
            const ResultRun& run_a,
            const ResultRun& run_b,
            const std::vector<UserPrompt>& user_prompts
        );
        void compute_similarities(CompareResultRunsViewModel& vm);
    }

    namespace  create_user_prompt {
        struct CreateUserPromptViewModel
        {
            std::string prompt = "";
            errors::DisplayError prompt_error;
        };

        CreateUserPromptViewModel init();
        void create_user_prompt(
            dba::DBAState& dba_state,
            CreateUserPromptViewModel& create_user_prompt_vm,
            user_prompt::UserPromptViewModel& user_prompt_vm
        );
        void validate(
            CreateUserPromptViewModel& create_user_prompt_vm
        );
    }

    namespace edit_user_prompt {
        struct EditUserPromptViewModel
        {
            int64_t id = 0;
            std::string prompt = "";
            errors::DisplayError prompt_error;
        };

        void prepare(EditUserPromptViewModel& vm, const UserPrompt& user_prompt);
        void update_user_prompt(
            dba::DBAState& dba_state,
            EditUserPromptViewModel& vm,
            user_prompt::UserPromptViewModel& user_prompt_vm
        );
        void validate(EditUserPromptViewModel& vm);
    }
}
