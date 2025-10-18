#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ui.h"
#include "app.h"
#include "theme.h"

using namespace std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int create_window(app::AppState& state)
{
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state.window = glfwCreateWindow(
        state.width,
        state.height,
        "Prompt Workbench",
        NULL,
        NULL
    );

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

int init_glad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    return 0;
}

void set_fonts(ImGuiIO *io)
{
    io->Fonts->Clear(); 
    path exePath = current_path();
    path fontPath = exePath / "fonts" / "Inter-Regular.ttf";
    ImFont* font_regular = io->Fonts->AddFontFromFileTTF(
        fontPath.string().c_str(),
        16.0f
    );
    io->FontDefault = font_regular;
}

void init_imgui(app::AppState& state)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    theme::apply_color_scheme();

    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    set_fonts(&io);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(monitor);

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontSizeBase = 16.0f;
    style.FontScaleDpi = main_scale;

    ImGui_ImplGlfw_InitForOpenGL(state.window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void app::render_loop(AppState& app_state, dba::DBAState& dba_state)
{
    ImGuiIO& io = ImGui::GetIO(); 
    vm::test_suites::TestSuitesViewModel test_suites_vm = 
        vm::test_suites::init(dba_state);

    vm::user_prompts::UserPromptsViewModel user_prompts_vm =
        vm::user_prompts::init(dba_state);

    while (!glfwWindowShouldClose(app_state.window))
    {
        glfwWaitEvents();
        if (glfwGetWindowAttrib(app_state.window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ui::components::main_frame(
            dba_state,
            test_suites_vm,
            user_prompts_vm
        );

        ImGui::Render();

        glClearColor(
            app_state.clearColor.x * app_state.clearColor.w,
            app_state.clearColor.y * app_state.clearColor.w,
            app_state.clearColor.z * app_state.clearColor.w,
            app_state.clearColor.w
        );

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(app_state.window);
    }
}

int app::init(AppState& state)
{
    int create_window_response = create_window(state);

    if (create_window_response == -1)
    {
        std::cout << "GLFW window creation error" << std::endl;
        return -1;
    }

    int init_glad_res = init_glad();

    if (init_glad_res == -1)
    {
        std::cout << "GLAD init error" << std::endl;
        return -1;
    }

    init_imgui(state);

    return 0;
}

void app::deinit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();    
    glfwTerminate();
}
