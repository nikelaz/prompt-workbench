#include <string>

namespace api {
    std::string openai_ask(
        const std::string& api_endpoint, 
        const std::string& api_key,
        const std::string& model,
        const std::string& system_prompt,
        const std::string& user_prompt
    );
}
