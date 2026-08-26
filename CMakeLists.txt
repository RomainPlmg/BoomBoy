cmake_minimum_required(VERSION 3.30.0)

project(BoomBoy
        VERSION 0.1.0
        DESCRIPTION "A tiny Game Boy emulator!"
        LANGUAGES C CXX)

include(FetchContent)

# ============================================================================
# C++ Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_command.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Define executable
# ============================================================================
add_executable(BoomBoy
        src/Main.cpp
        src/core/Log.cpp
        src/core/Timer.cpp
        src/components/Cartridge.cpp
)

target_include_directories(BoomBoy PRIVATE ${PROJECT_SOURCE_DIR}/src/)

# ============================================================================
# Dependencies
# ============================================================================
find_package(Freetype REQUIRED)

# SDL3
FetchContent_Declare(
        sdl
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.14
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        SYSTEM
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sdl)

# SpdLog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.x
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        SYSTEM
)
FetchContent_MakeAvailable(spdlog)

# ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.9
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        SYSTEM
)
FetchContent_MakeAvailable(imgui)
file(GLOB IMGUI_SRC
    ${imgui_SOURCE_DIR}/*.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imgui_SOURCE_DIR}/misc
)
target_link_libraries(imgui PRIVATE SDL3::SDL3 Freetype::Freetype)

target_link_libraries(BoomBoy PRIVATE SDL3::SDL3 spdlog imgui)