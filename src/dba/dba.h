#pragma once

#include <hdr/sqlite_modern_cpp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <memory>
#include <optional>

struct TestSuite
{
    int64_t id;
    std::string title;
    std::string description;
    std::string system_prompt;
    std::string model;
    std::optional<int64_t> prompt_id;
};

struct OpenPrompt
{
    int64_t id;
    std::string file_path;
};

struct UserPrompt
{
    int64_t id;
    std::string prompt;
    int64_t test_suite_id;
};

struct ResultRun
{
    int64_t id;
    std::string date;
    int64_t test_suite_id;
    std::string system_prompt;
    std::optional<std::string> title;
};

struct Answer
{
    int64_t id;
    std::string answer;
    int64_t user_prompt_id;
    int64_t result_run_id;
    std::vector<float> embedding;
};

struct Settings
{
    std::string api_endpoint;
    std::string api_key;
};

namespace dba
{
    struct DBAState
    {
        std::unique_ptr<sqlite::database> db;
    };

    // Initialization
    void create_tables(DBAState& state);
    bool init(DBAState& state, const std::string& db_path);
    void deinit(DBAState& state);

    // Test Suites
    std::optional<int64_t> create_test_suite(
        DBAState& state,
        const std::string& title,
        const std::string& description,
        const std::string& system_prompt,
        const std::string& model,
        std::optional<int64_t> prompt_id = std::nullopt
    );
    std::optional<TestSuite> get_test_suite(DBAState& state, int64_t id);
    std::vector<TestSuite> get_all_test_suites(DBAState& state);
    std::vector<TestSuite> get_test_suites_for_prompt(DBAState& state, int64_t prompt_id);
    bool update_test_suite(
        DBAState& state,
        int64_t id,
        const std::optional<std::string>& title,
        const std::optional<std::string>& description,
        const std::optional<std::string>& system_prompt,
        const std::optional<std::string>& model
    );
    bool delete_test_suite(DBAState& state, int64_t id);

    // User Prompts
    std::optional<int64_t> create_user_prompt(
        DBAState& state,
        const std::string& prompt, 
        int64_t test_suite_id
    );
    std::optional<UserPrompt> get_user_prompt(DBAState& state, int64_t id);   
    std::vector<UserPrompt> get_all_user_prompts(DBAState& state, int64_t test_suite_id); 
    bool update_user_prompt(
        DBAState& state,
        int64_t id,
        const std::optional<std::string>& prompt, 
        std::optional<int64_t> test_suite_id
    );
    bool delete_user_prompt(DBAState& state, int64_t id);

    // Result Runs
    std::optional<int64_t> create_result_run(
        DBAState& state,
        const std::string& date,
        int64_t test_suite_id,
        const std::string& system_prompt
    );
    std::optional<ResultRun> get_result_run(DBAState& state, int64_t id);   
    std::vector<ResultRun> get_all_result_runs(DBAState& state, int64_t test_suite_id); 
    bool update_result_run(
        DBAState& state,
        int64_t id,
        const std::optional<std::string>& date, 
        std::optional<int64_t> test_suite_id
    );
    bool delete_result_run(DBAState& state, int64_t id);
    bool update_result_run_title(DBAState& state, int64_t id, const std::string& title);

    // Answers
    std::optional<int64_t> create_answer(
        DBAState& state,
        const std::string& answer, 
        int64_t user_prompt_id,
        int64_t result_run_id
    );
    std::optional<Answer> get_answer(DBAState& state, int64_t id);   
    std::vector<Answer> get_all_answers(DBAState& state, int64_t result_run_id); 
    bool update_answer(
        DBAState& state,
        int64_t id,
        const std::optional<std::string>& answer, 
        std::optional<int64_t> user_prompt_id,
        std::optional<int64_t> result_run_id
    );
    bool delete_answer(DBAState& state, int64_t id);
    void store_embedding(DBAState& state, int64_t answer_id, const std::vector<float>& embedding);

    // Settings
    Settings get_settings(DBAState& state);
    void upsert_settings(DBAState& state, const std::string& api_endpoint, const std::string& api_key);

    // Open Prompts
    std::optional<int64_t> open_prompt(DBAState& state, const std::string& file_path);
    std::vector<OpenPrompt> get_all_open_prompts(DBAState& state);
    void close_prompt(DBAState& state, int64_t id);
}
