# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an OpenGL learning project following the tutorial book *Computer Graphics Programming in OpenGL with C++*. It contains example programs and exercises organized by chapter/program number.

## Build Commands

```bash
# Configure and build (from project root)
cmake -B build
cmake --build build

# Run the application
./build/app
```

The project uses CMake 3.4+ and requires C++17. All dependencies (GLFW, GLEW, GLM) are bundled in `third-party/`.

## Architecture

**Entry Point:** `src/app.cpp` - contains `main()` which calls `run<WindowType>()` template function. Change the template argument to switch between different example programs.

**Window System:**
- `WindowBase` - Abstract base class providing GLFW window management, render loop, and frame rate stats
- Derived classes (`WindowOnePoint`, `WindowOneTriangle`, `WindowOneCube`) implement specific chapter examples
- Each window class overrides `Init()` for setup and `Display(double current_time)` for rendering

**Key Files:**
- `src/include/launch.h` - `run<Window>()` template that initializes GLFW/GLEW and runs the main loop
- `src/include/opengl.h` - Single include for all OpenGL headers (GLEW, GLFW, GLM)
- `src/include/Utils.h` - Shader loading utilities (`CreateShaderProgram`, `ReadShaderSource`)
- `src/include/debug.h` - OpenGL error checking macros (`GLCall`, `SHADER_LOG`, `PROGRAM_LOG`)

**Shaders:** Located in `res/shader/`, named by chapter/program (e.g., `vertShader-4.5.glsl`)

## Adding a New Example

1. Create a new header in `src/include/` extending `WindowBase`
2. Create corresponding `.cpp` in `src/`
3. Add shaders to `res/shader/` if needed
4. Update `src/app.cpp` to use the new window class

## Workflow Rules

1. **Commit immediately**: After completing any modifications, commit the changes with git immediately
2. **Build verification only**: This is a graphics program. Verify by compiling (`cmake --build build`), but do not run the application for verification
