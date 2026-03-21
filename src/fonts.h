#pragma once

#include "imgui.h"

namespace fonts {
    struct Fonts
    {
        ImFont* regular_small;
        ImFont* regular_medium;
        ImFont* heading;
    };

    Fonts load(ImGuiIO *io);
}
