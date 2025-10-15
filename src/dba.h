#pragma once

#include <hdr/sqlite_modern_cpp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <memory>

struct TestSuite
{
    int id;
    std::string title;
    std::string description;
    std::string system_prompt;
    std::string model;
};

struct UserPrompt
{
    int id;
    std::string prompt;
    int test_suite_id;
};

struct ResultRun
{
    int id;
    std::string date;
    int test_suite_id;
};

struct Answer
{
    int id;
    std::string answer;
    int result_run_id;
};

namespace dba
{
    static std::unique_ptr<sqlite::database> db;
    
    // Initialization
    void create_tables();
    bool init(const std::string& db_path);

    // Test Suites 
    int create_test_suite(
        const std::string& title, 
        const std::string& description,
        const std::string& system_prompt,
        const std::string& model
    );
    std::unique_ptr<TestSuite> get_test_suite(int id);
    std::vector<TestSuite> get_all_test_suites();
    void update_test_suite(
        int id,
        const std::string* title = nullptr,
        const std::string* description = nullptr,
        const std::string* system_prompt = nullptr,
        const std::string* model = nullptr
    );
    void delete_test_suite(int id);

    // User Prompts
    int create_user_prompt(
        const std::string& prompt, 
        int test_suite_id
    );
    std::unique_ptr<UserPrompt> get_user_prompt(int id);   
    std::vector<UserPrompt> get_all_user_prompts(); 
    void update_user_prompt(
        int id,
        const std::string* prompt = nullptr, 
        // Todo: I don't like this at -1, seems arbitrary,
        // research a better option
        int test_suite_id = -1
    );
    void delete_user_prompt(int id);
}
