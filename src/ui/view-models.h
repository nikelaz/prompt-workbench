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
        };

        struct RunTestsViewModel {
            std::atomic<bool> is_running{false};
            std::vector<RunResult> pending_results;
            std::string run_date;
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
}
