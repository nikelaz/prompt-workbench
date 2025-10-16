#pragma once

#include <hdr/sqlite_modern_cpp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <memory>

struct TestSuite
{
    int64_t id;
    std::string title;
    std::string description;
    std::string system_prompt;
    std::string model;
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
};

struct Answer
{
    int64_t id;
    std::string answer;
    int64_t result_run_id;
};

namespace dba
{
    extern std::unique_ptr<sqlite::database> db;
    
    // Initialization
    void create_tables();
    bool init(const std::string& db_path);

    // Test Suites 
    std::optional<int64_t> create_test_suite(
        const std::string& title, 
        const std::string& description,
        const std::string& system_prompt,
        const std::string& model
    );
    std::optional<TestSuite> get_test_suite(int64_t id);
    std::vector<TestSuite> get_all_test_suites();
    bool update_test_suite(
        int64_t id,
        const std::optional<std::string>& title,
        const std::optional<std::string>& description,
        const std::optional<std::string>& system_prompt,
        const std::optional<std::string>& model
    );
    bool delete_test_suite(int64_t id);

    // User Prompts
    std::optional<int64_t> create_user_prompt(
        const std::string& prompt, 
        int64_t test_suite_id
    );
    std::optional<UserPrompt> get_user_prompt(int64_t id);   
    std::vector<UserPrompt> get_all_user_prompts(); 
    bool update_user_prompt(
        int64_t id,
        const std::optional<std::string>& prompt, 
        std::optional<int64_t> test_suite_id
    );
    bool delete_user_prompt(int64_t id);
}
