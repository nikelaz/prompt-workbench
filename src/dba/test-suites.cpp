#include "dba.h"
#include <iostream>

using std::string;
using std::optional;
using std::vector;

optional<int64_t> dba::create_test_suite(
    const string& title, 
    const string& description,
    const string& system_prompt,
    const string& model
)
{
    if (!db) throw std::runtime_error("DB not initialized yet");

    try
    {
        (*db) << R"(
            INSERT INTO test_suites
            (title, description, system_prompt, model)
            values (?, ?, ?, ?)
        )"
            << title
            << description
            << system_prompt
            << model;

        return db->last_insert_rowid();
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

optional<TestSuite> dba::get_test_suite(int64_t id)
{
    optional<TestSuite> result;

    (*db) << R"( 
        SELECT
        title, description, system_prompt, model
        FROM test_suites
        WHERE id == ?;
    )"
        << id
        >> [&](
            string title,
            string description,
            string system_prompt,
            string model
        )
        {
            result = TestSuite {
                id,
                title,
                description,
                system_prompt,
                model
            };
        };

    return result;
}

vector<TestSuite> dba::get_all_test_suites()
{
    vector<TestSuite> test_suites;

    (*db) << R"( 
        SELECT
        id, title, description, system_prompt, model
        FROM test_suites;
    )"
        >> [&](
            int64_t id,
            string title,
            string description,
            string system_prompt,
            string model
        )
        {
            test_suites.emplace_back(
                id,
                title,
                description,
                system_prompt,
                model
            ); 
        };

    return test_suites;
}


bool dba::update_test_suite(
    int64_t id,
    const optional<string>& title,
    const optional<string>& description,
    const optional<string>& system_prompt,
    const optional<string>& model
)
{    
    try
    {
        std::string sql = "UPDATE test_suites SET ";
        std::vector<std::pair<std::string, std::string>> updates;

        if (title) updates.emplace_back("title", *title);
        if (description) updates.emplace_back("description", *description);
        if (system_prompt) updates.emplace_back("system_prompt", *system_prompt);
        if (model) updates.emplace_back("model", *model);

        if (updates.empty())
        {
            return false;
        }

        bool first = true;

        for (auto& [col, val] : updates)
        {
            if (!first) sql += ", ";
            sql += col + " = ?";
            first = false;
        }

        sql += " WHERE id = ?;";

        auto stmt = (*db) << sql;
        for (auto& [col, val] : updates)
        {
            stmt << val;
        }

        stmt << id;

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return false;
    }
}

bool dba::delete_test_suite(int64_t id)
{
    try
    {
        (*db) << R"(
            DELETE FROM test_suites
            WHERE id = ?
        )"
            << id;

        return true;
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return false;
    }
}
