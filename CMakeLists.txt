cmake_minimum_required(VERSION 3.25)
project(app LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/glfw)
set(CPR_CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
# MSVC natively supports <filesystem> — skip cpr's try_compile probe which fails in CMake subprocesses on Windows
set(STD_FS_SUPPORTED TRUE CACHE INTERNAL "" FORCE)
add_subdirectory(external/cpr)
add_subdirectory(external/json)

# llama.cpp — disable unneeded targets
set(LLAMA_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(LLAMA_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LLAMA_BUILD_SERVER   OFF CACHE BOOL "" FORCE)
add_subdirectory(external/llama.cpp)

# Download embedding model at configure time if missing
set(EMBED_MODEL_FILENAME "nomic-embed-text-v1.5.Q8_0.gguf")
set(EMBED_MODEL_URL "https://huggingface.co/nomic-ai/nomic-embed-text-v1.5-GGUF/resolve/main/${EMBED_MODEL_FILENAME}")
set(EMBED_MODEL_SRC "${CMAKE_SOURCE_DIR}/assets/models/${EMBED_MODEL_FILENAME}")
if(NOT EXISTS "${EMBED_MODEL_SRC}")
    message(STATUS "Downloading embedding model (~274 MB)...")
    file(DOWNLOAD "${EMBED_MODEL_URL}" "${EMBED_MODEL_SRC}" SHOW_PROGRESS
         STATUS EMBED_DL_STATUS)
    list(GET EMBED_DL_STATUS 0 EMBED_DL_CODE)
    if(NOT EMBED_DL_CODE EQUAL 0)
        message(WARNING "Failed to download embedding model. Meaning Diff feature will be unavailable.")
        file(REMOVE "${EMBED_MODEL_SRC}")
    endif()
endif()

find_package(Freetype REQUIRED CONFIG)

add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/misc/freetype/imgui_freetype.cpp
)
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends external/imgui/misc/freetype ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(imgui PUBLIC glfw ${FREETYPE_LIBRARIES})
target_compile_definitions(imgui PUBLIC
    IMGUI_USE_WCHAR32
    IMGUI_ENABLE_FREETYPE
)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

find_package(OpenGL REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(app
    src/main.cpp
    src/app.cpp
    src/helpers.cpp
    src/api.cpp
    src/embedding.cpp
    src/fonts.cpp
    src/dba/dba.cpp
    src/dba/test-suites.cpp
    src/dba/user-prompts.cpp
    src/dba/result-runs.cpp
    src/dba/answers.cpp
    src/dba/settings.cpp
    src/ui/theme.cpp
    src/ui/view-models.cpp
    src/ui/routing.cpp
    src/ui/components/button.cpp
    src/ui/components/confirm-dialog.cpp
    src/ui/components/card.cpp
    src/ui/components/top-bar.cpp
    src/ui/components/page-header.cpp
    src/ui/components/sidebar.cpp
    src/ui/components/main-frame.cpp
    src/ui/components/input.cpp
    src/ui/components/input-error.cpp
    src/ui/components/select.cpp
    src/ui/components/content-container.cpp
    src/ui/views/test-suites.cpp
    src/ui/views/test-suite-details.cpp
    src/ui/views/user-prompt-details.cpp
    src/ui/views/user-prompts-list.cpp
    src/ui/views/result-runs-list.cpp
    src/ui/views/result-run-details.cpp
    src/ui/views/answer-details.cpp
    src/ui/views/compare-result-runs.cpp
    src/ui/views/system-prompt-comparison.cpp
    src/ui/views/create-test-suites.cpp
    src/ui/views/edit-test-suite.cpp
    src/ui/views/create-user-prompts.cpp
    src/ui/views/edit-user-prompt.cpp
    src/ui/views/api-credentials.cpp
)

target_include_directories(app PRIVATE src/ src/dba src/ui src/platform)

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
  llama
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_package(X11 REQUIRED)
  target_link_libraries(app PRIVATE X11 unistring)
  target_sources(app PRIVATE src/platform/accent_color_linux.cpp)
else()
  target_sources(app PRIVATE src/platform/accent_color_stub.cpp)
endif()

add_custom_command(TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/fonts
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/fonts/Inter-Regular.ttf
        $<TARGET_FILE_DIR:app>/fonts/Inter-Regular.ttf
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/assets/fonts/TwemojiMozilla.ttf
        $<TARGET_FILE_DIR:app>/fonts/TwemojiMozilla.ttf
)

if(EXISTS "${EMBED_MODEL_SRC}")
    add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/models
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${EMBED_MODEL_SRC}"
            $<TARGET_FILE_DIR:app>/models/embed.gguf
    )
endif()
