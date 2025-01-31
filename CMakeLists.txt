cmake_minimum_required(VERSION 3.10)
project(ImGuiWasmExample)

# Include OpenGL and GLFW if using them in the build
find_package(OpenGL REQUIRED)

# Set Emscripten toolchain file
set(CMAKE_TOOLCHAIN_FILE "/home/vagrant/fun/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")

# CMake compiler for Emscripten
set(CMAKE_C_COMPILER emcc)
set(CMAKE_CXX_COMPILER em++)

set(CMAKE_CXX_STANDARD 11)

# Include directories for ImGui and its backends
include_directories(${CMAKE_SOURCE_DIR}/imgui)
include_directories(${CMAKE_SOURCE_DIR}/imgui/backends)

# Bad method atm (fix later)
include_directories("/usr/include/GLFW")

# Add source files for ImGui and your project
set(IMGUI_SOURCES
    src/example.cpp
    imgui_local/imgui.cpp
    imgui_local/imgui_draw.cpp
    imgui_local/imgui_demo.cpp
    imgui_local/imgui_widgets.cpp
    imgui_local/imgui_tables.cpp
    imgui_local/backends/imgui_impl_glfw.cpp
    imgui_local/backends/imgui_impl_opengl3.cpp
)

message("Toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# Emscripten-specific flags
set(CMAKE_CXX_FLAGS "-lGL -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1 -O2 -s WASM=1")

# Define the executable
add_executable(imgui ${IMGUI_SOURCES})

# Set the output directory to 'out'
set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/out")
file(MAKE_DIRECTORY ${OUTPUT_DIR})

# Set target properties for generating .js and .wasm
set_target_properties(imgui PROPERTIES
    OUTPUT_NAME "imgui"
    SUFFIX ".js"
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

# Copy the imgui.html file from the root directory to the 'out' directory
add_custom_command(TARGET imgui POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/imgui.html
    ${OUTPUT_DIR}/imgui.html
)

# Add necessary flags for emcc to generate both .wasm and .js
set_target_properties(imgui PROPERTIES
    LINK_FLAGS "-o ${OUTPUT_DIR}/imgui.js"
)

