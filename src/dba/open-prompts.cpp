#include "dba.h"
#include <iostream>

using std::string;
using std::optional;
using std::vector;

optional<int64_t> dba::open_prompt(DBAState& state, const string& file_path)
{
    try
    {
        (*state.db) << "INSERT OR IGNORE INTO open_prompts (file_path) VALUES (?)" << file_path;

        optional<int64_t> result;
        (*state.db) << "SELECT id FROM open_prompts WHERE file_path = ?" << file_path
            >> [&](int64_t id) { result = id; };
        return result;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

vector<OpenPrompt> dba::get_all_open_prompts(DBAState& state)
{
    vector<OpenPrompt> prompts;
    (*state.db) << "SELECT id, file_path FROM open_prompts ORDER BY id"
        >> [&](int64_t id, string file_path)
        {
            prompts.emplace_back(OpenPrompt{ id, std::move(file_path) });
        };
    return prompts;
}

void dba::close_prompt(DBAState& state, int64_t id)
{
    try
    {
        (*state.db) << "DELETE FROM open_prompts WHERE id = ?" << id;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}
