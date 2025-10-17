#include "dba.h"
#include <iostream>
#include <string>

using std::string;
using std::optional;
using std::vector;

optional<int64_t> dba::create_result_run(
    const string& date, 
    int64_t test_suite_id
)
{
    try
    {
        (*db) << R"(
            INSERT INTO result_runs
            (date, test_suite_id)
            values (?, ?)
        )"
            << date
            << test_suite_id;

        return db->last_insert_rowid();
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

optional<ResultRun> dba::get_result_run(int64_t id) 
{
    optional<ResultRun> result;

    (*db) << R"( 
        SELECT
        date, test_suite_id
        FROM result_runs 
        WHERE id == ?;
    )"
        << id
        >> [&](
            string date,
            int64_t test_suite_id
        )
        {
            result = ResultRun {
                id,
                date,
                test_suite_id,
            };
        };

    return result;
}

vector<ResultRun> dba::get_all_result_runs() 
{
    vector<ResultRun> result_runs;

    (*db) << R"( 
        SELECT
        id, date, test_suite_id
        FROM result_runs;
    )"
        >> [&](
            int64_t id,
            string date,
            int64_t test_suite_id
        )
        {
            result_runs.emplace_back(
                id,
                date,
                test_suite_id
            ); 
        };

    return result_runs;
}

bool dba::update_result_run(
    int64_t id,
    const optional<string>& date, 
    optional<int64_t> test_suite_id
)
{ 
    try
    {
        string sql = "UPDATE result_runs SET ";
        vector<std::pair<string, string>> updates;

        if (date)
        {
            updates.emplace_back("date", *date);
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

bool dba::delete_result_run(int64_t id)
{
    try
    {
        (*db) << R"(
            DELETE FROM result_runs
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
