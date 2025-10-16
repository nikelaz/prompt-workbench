#include "dba.h"
#include <iostream>
#include <string>

using std::string;
using std::optional;
using std::vector;

optional<int64_t> dba::create_user_prompt(
    const string& prompt, 
    int64_t test_suite_id
)
{
    try
    {
        (*db) << R"(
            INSERT INTO user_prompts
            (prompt, test_suite_id)
            values (?, ?)
        )"
            << prompt
            << test_suite_id;

        return db->last_insert_rowid();
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

optional<UserPrompt> dba::get_user_prompt(int64_t id) 
{
    optional<UserPrompt> result;

    (*db) << R"( 
        SELECT
        prompt, test_suite_id
        FROM user_prompts
        WHERE id == ?;
    )"
        << id
        >> [&](
            string prompt,
            int64_t test_suite_id
        )
        {
            result = UserPrompt {
                id,
                prompt,
                test_suite_id,
            };
        };

    return result;
}

vector<UserPrompt> dba::get_all_user_prompts() 
{
    vector<UserPrompt> user_prompts;

    (*db) << R"( 
        SELECT
        id, prompt, test_suite_id
        FROM user_prompts;
    )"
        >> [&](
            int64_t id,
            string prompt,
            int64_t test_suite_id
        )
        {
            user_prompts.emplace_back(
                id,
                prompt,
                test_suite_id
            ); 
        };

    return user_prompts;
}

bool dba::update_user_prompt(
    int64_t id,
    const optional<string>& prompt, 
    optional<int64_t> test_suite_id
)
{ 
    try
    {
        string sql = "UPDATE user_prompts SET ";
        vector<std::pair<string, string>> updates;

        if (prompt)
        {
            updates.emplace_back("prompt", *prompt);
        }

        if (test_suite_id)
        {
            updates.emplace_back("test_suite_id", std::to_string(*test_suite_id));
        }

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

bool dba::delete_user_prompt(int64_t id)
{
    try
    {
        (*db) << R"(
            DELETE FROM user_prompts
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
