#pragma once

#include <string>

namespace errors {
    struct DisplayError {
        bool has_error = false;
        std::string message = "";
    };
}
