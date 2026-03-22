#include "dba.h"
#include <iostream>
#include <string>

using std::string;
using std::optional;
using std::vector;

optional<int64_t> dba::create_answer(
    DBAState& state,
    const string& answer,
    int64_t user_prompt_id,
    int64_t result_run_id
)
{
    try
    {
        (*state.db) << R"(
            INSERT INTO answers
            (answer, user_prompt_id, result_run_id)
            values (?, ?, ?)
        )"
            << answer
            << user_prompt_id
            << result_run_id;

        return state.db->last_insert_rowid();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

void dba::store_embedding(
    DBAState& state,
    int64_t answer_id,
    const vector<float>& embedding
)
{
    try
    {
        (*state.db) << R"(
            INSERT OR REPLACE INTO answer_embeddings (answer_id, embedding)
            VALUES (?, ?)
        )"
            << answer_id
            << embedding;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

optional<Answer> dba::get_answer(DBAState& state, int64_t id)
{
    optional<Answer> result;

    (*state.db) << R"(
        SELECT
        answer, user_prompt_id, result_run_id
        FROM answers
        WHERE id == ?;
    )"
        << id
        >> [&](
            string answer,
            int64_t user_prompt_id,
            int64_t result_run_id
        )
        {
            result = Answer {
                id,
                answer,
                user_prompt_id,
                result_run_id,
            };
        };

    return result;
}

vector<Answer> dba::get_all_answers(DBAState& state, int64_t result_run_id)
{
    vector<Answer> answers;

    (*state.db) << R"(
        SELECT
            a.id, a.answer, a.user_prompt_id, a.result_run_id,
            ae.embedding
        FROM answers a
        LEFT JOIN answer_embeddings ae ON ae.answer_id = a.id
        WHERE a.result_run_id = ?;
    )"
        << result_run_id
        >> [&](
            int64_t id,
            string answer,
            int64_t user_prompt_id,
            int64_t run_id,
            vector<float> embedding
        )
        {
            answers.emplace_back(Answer{
                id,
                std::move(answer),
                user_prompt_id,
                run_id,
                std::move(embedding)
            });
        };

    return answers;
}

bool dba::update_answer(
    DBAState& state,
    int64_t id,
    const optional<string>& answer,
    optional<int64_t> user_prompt_id,
    optional<int64_t> result_run_id
)
{
    try
    {
        string sql = "UPDATE answers SET ";
        vector<std::pair<string, string>> updates;

        if (answer)
        {
            updates.emplace_back("answer", *answer);
        }

        if (user_prompt_id)
        {
            updates.emplace_back("user_prompt_id", std::to_string(*user_prompt_id));
        }

        if (result_run_id)
        {
            updates.emplace_back("result_run_id", std::to_string(*result_run_id));
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

        auto stmt = (*state.db) << sql;
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

bool dba::delete_answer(DBAState& state, int64_t id)
{
    try
    {
        (*state.db) << R"(
            DELETE FROM answers
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
