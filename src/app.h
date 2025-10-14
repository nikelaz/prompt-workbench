#pragma once

#include "imgui.h"
#include <GLFW/glfw3.h>

namespace app {
    struct AppState {
        GLFWwindow* window = nullptr;
        int width = 1280;
        int height = 720;
        ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    inline AppState state;

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    int createWindow(); 
    int initGlad();
    void setFonts(ImGuiIO *io);
    void initImGui();
    void renderLoop();
    int init(AppState&);
    int shutdown();
}
