cmake_minimum_required(VERSION 3.25)
project(app LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/glfw)
add_subdirectory(external/cpr)
add_subdirectory(external/json)

add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends)
target_link_libraries(imgui PUBLIC glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(app
    src/main.cpp
    src/app.cpp
    src/helpers.cpp
    src/api.cpp
    src/fonts.cpp
    src/dba/dba.cpp
    src/dba/test-suites.cpp
    src/dba/user-prompts.cpp
    src/dba/result-runs.cpp
    src/dba/answers.cpp
    src/ui/theme.cpp
    src/ui/view-models.cpp
    src/ui/routing.cpp
    src/ui/components/button.cpp
    src/ui/components/card.cpp
    src/ui/components/top-bar.cpp
    src/ui/components/sidebar.cpp
    src/ui/components/main-frame.cpp
    src/ui/components/input.cpp
    src/ui/components/input-error.cpp
    src/ui/views/test-suites.cpp
    src/ui/views/test-suite-details.cpp
    src/ui/views/user-prompt-details.cpp
    src/ui/views/result-run-details.cpp
    src/ui/views/create-test-suites.cpp
    src/ui/views/create-user-prompts.cpp
)

target_include_directories(app PRIVATE src/ src/dba src/ui)

target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/external/sqlite_modern_cpp
    ${SQLite3_INCLUDE_DIRS}
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(app PRIVATE
  glad
  OpenGL::GL
  glfw
  imgui
  nlohmann_json::nlohmann_json
  cpr::cpr
  ${SQLite3_LIBRARIES}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(X11 REQUIRED)
  target_link_libraries(app PRIVATE X11 unistring)
endif()

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/fonts
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/fonts/Inter-Regular.ttf
        $<TARGET_FILE_DIR:app>/fonts/Inter-Regular.ttf
)
