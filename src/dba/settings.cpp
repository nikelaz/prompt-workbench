#include "dba.h"
#include <iostream>

using std::string;

Settings dba::get_settings(DBAState& state)
{
    Settings result;

    (*state.db) << R"(
        SELECT api_endpoint, api_key
        FROM settings
        WHERE id = 1;
    )"
        >> [&](string api_endpoint, string api_key)
        {
            result = Settings { api_endpoint, api_key };
        };

    return result;
}

void dba::upsert_settings(
    DBAState& state,
    const string& api_endpoint,
    const string& api_key
)
{
    try
    {
        (*state.db) << R"(
            UPDATE settings
            SET api_endpoint = ?, api_key = ?
            WHERE id = 1;
        )"
            << api_endpoint
            << api_key;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}
