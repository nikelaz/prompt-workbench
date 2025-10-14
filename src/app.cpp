#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui.h"
#include "app.h"

void app::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int app::createWindow() {
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state.window = glfwCreateWindow(state.width, state.height, "Hello World", NULL, NULL);

    if (!state.window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(state.window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(state.window, framebuffer_size_callback);

    return 0;
}

int app::initGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    return 0;
}

void app::setFonts(ImGuiIO *io) {
    io->Fonts->Clear(); 
    std::filesystem::path exePath = std::filesystem::current_path();
    std::filesystem::path fontPath = exePath / "fonts" / "Inter-Regular.ttf";
    ImFont* font_regular = io->Fonts->AddFontFromFileTTF(fontPath.c_str(), 16.0f);
    io->FontDefault = font_regular;
}

void app::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    setFonts(&io);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(monitor);

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontSizeBase = 16.0f;
    style.FontScaleDpi = main_scale;

    ImGui_ImplGlfw_InitForOpenGL(state.window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void app::renderLoop() {
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGuiIO& io = ImGui::GetIO(); 

    while (!glfwWindowShouldClose(state.window))
    {
        glfwWaitEvents();
        if (glfwGetWindowAttrib(state.window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // main frame
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            ImGui::Begin(
                "MainWindow",
                nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoDecoration
            ); 

            ui::button("Hello Button!");
            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate
            );

            ImGui::End();
        }

        ImGui::Render();

        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(state.window);
    }
}

int app::init(AppState& inputOptions)
{
    state = inputOptions;
    int createWindowRes = createWindow();

    if (createWindowRes == -1)
    {
        std::cout << "GLFW window creation error" << std::endl;
        return -1;
    }

    int initGladRes = initGlad();

    if (initGladRes == -1)
    {
        std::cout << "GLAD init error" << std::endl;
        return -1;
    }

    initImGui();

    return 0;
}

int app::shutdown()
{
    glfwTerminate();

    return 0;
}
