# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Prompt Workbench is a C++23 desktop GUI application for testing and evaluating AI prompts. It uses ImGui (immediate-mode GUI) with OpenGL rendering, SQLite for persistence, and an OpenAI-compatible API for running prompts.

## Build & Run

All external dependencies are git submodules. Initialize them before building:
```bash
git submodule update --init --recursive
```

Build only:
```bash
mkdir build && cd build && cmake .. && cmake --build . -j$(nproc)
```

Build and run (via scripts):
```bash
./build.sh   # build only
./run.sh     # build + run
```

Executable output: `./build/bin/app`

### System Dependencies

**RHEL/Fedora:** `mesa-libGL-devel glfw-devel libX11-devel libXrandr-devel libXi-devel libXinerama-devel libXcursor-devel wayland-devel sqlite-devel openssl-devel`

**Ubuntu/Debian:** `libgl1-mesa-dev libglfw3-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libwayland-dev libsqlite3-dev libssl-dev`

## Architecture

### Layer Structure

```
UI Views → ViewModels → DBA (Database Abstraction Layer) → SQLite
UI Views → API Layer → OpenAI-compatible HTTP endpoint
```

### Key Namespaces & Files

- **`dba::`** (`src/dba/`) — All SQLite operations. Each entity has its own `.cpp` file (`test-suites.cpp`, `user-prompts.cpp`, `result-runs.cpp`, `answers.cpp`). Core structs defined in `dba.h`.
- **`ui::`** (`src/ui/`) — All rendering code.
  - `views/` — Full-screen views (one per route).
  - `components/` — Reusable widgets (`button`, `card`, `input`, `top_bar`, `sidebar`).
  - `view-models.h/cpp` — Per-view state and form validation.
  - `routing.h/cpp` — Stack-based navigation state machine.
  - `theme.h/cpp` — Color constants.
- **`api::`** (`src/api.h/cpp`) — HTTP calls to OpenAI-compatible endpoints via CPR.
- **`app::`** (`src/app.h/cpp`) — GLFW window init, OpenGL context, ImGui setup, render loop.

### Data Model

```
TestSuite (1) → (N) UserPrompt
TestSuite (1) → (N) ResultRun
UserPrompt (1) → (N) Answer
ResultRun  (1) → (N) Answer
```

### Render Loop

Each frame: `glfwWaitEvents` → `ImGui::NewFrame()` → `main_frame()` → route to active view → `ImGui::Render()` → swap buffers.

`main_frame()` (`src/ui/components/main-frame.cpp`) is the root component — it owns the ImGui dockspace and dispatches to the correct view based on router state.

### Routing

Routes are an enum in `routing.h`: `TEST_SUITES`, `TEST_SUITE_DETAILS`, `CREATE_TEST_SUITE`, `USER_PROMPT_DETAILS`, `CREATE_USER_PROMPTS`, `RESULT_RUN_DETAILS`. Navigate with `router::push(route)` and `router::back()`.

### ViewModels

ViewModels hold in-memory state for each view (selected items, form fields, validation errors). They are initialized once at startup from the DBA and passed by reference through the component tree. Use `DisplayError` struct (`errors.h`) for form validation errors.

## External Dependencies (submodules in `external/`)

- **imgui** — Immediate-mode GUI (docking branch)
- **glfw** — Window/input handling
- **glad** — OpenGL loader
- **cpr** — HTTP client
- **json** (nlohmann) — JSON parsing
- **sqlite_modern_cpp** — SQLite C++ wrapper
