#pragma once

#include "imgui.h"
#include <GLFW/glfw3.h>

namespace app {
    struct AppState {
        GLFWwindow* window = nullptr;
        int width = 1280;
        int height = 720;
        ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    int init(AppState&);
    void render_loop(AppState&);
    void deinit();
}
