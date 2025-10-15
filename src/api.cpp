#include "api.h"
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

std::string api::openai_ask(
    const std::string& api_endpoint, 
    const std::string& api_key,
    const std::string& model,
    const std::string& system_prompt,
    const std::string& user_prompt
)
{
    if (
        api_endpoint.empty() ||
        api_key.empty() ||
        model.empty() ||
        system_prompt.empty() ||
        user_prompt.empty()
    )
    {
        return "";
    }

    nlohmann::json payload;
    payload["model"] = model;
    payload["messages"] = {
        {
            { "role", "system" },
            { "content", system_prompt }
        },
        {
            { "role", "user" },
            { "content", user_prompt }
        }
    };

    try
    {
        cpr::Response r = cpr::Post(
            cpr::Url { api_endpoint },
            cpr::Header {
                { "Authorization", "Bearer " + api_key },
                { "Content-Type", "application/json" }
            },
            cpr::Body { payload.dump() }
        );
        
        if (r.status_code != 200) {
            return "";
        }
        
        auto response_json = nlohmann::json::parse(r.text);

        if (
            !response_json.contains("choices") ||
            response_json["choices"].empty()
        )
        {
            return "";
        }
        
        auto message_content = response_json["choices"][0]["message"]["content"];

        if (!message_content.is_string()) {
            return "";
        }
        
        return message_content.get<std::string>();
    }
    catch (const std::exception& e)
    {
        return "";
    }
}
