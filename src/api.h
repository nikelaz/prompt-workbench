#pragma once

#include <string>
#include <vector>

namespace api {
    std::string openai_ask(
        const std::string& api_endpoint,
        const std::string& api_key,
        const std::string& model,
        const std::string& system_prompt,
        const std::string& user_prompt
    );

    std::vector<std::string> get_models(
        const std::string& endpoint,
        const std::string& api_key
    );
}
