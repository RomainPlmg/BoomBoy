cmake_minimum_required(VERSION 3.30.0)

project(BoomBoy
        VERSION 0.1.0
        DESCRIPTION "A tiny Game Boy emulator!"
        LANGUAGES C CXX)

include(FetchContent)

# ============================================================================
# Options
# ============================================================================
option(BB_BUILD_TESTS "Download Mooneye test ROMs" OFF)

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
)

target_include_directories(BoomBoy PRIVATE ${PROJECT_SOURCE_DIR}/src/)

# ============================================================================
# Dependencies
# ============================================================================
add_subdirectory(src/core)
add_subdirectory(src/cartridge)
add_subdirectory(src/emulator)
add_subdirectory(src/platform)

target_link_libraries(BoomBoy PRIVATE gb_core gb_cartridge gb_emulator gb_platform)

# ============================================================================
# Tests
# ============================================================================
include(CTest)
if(BB_BUILD_TESTS)
        add_subdirectory(tests)
endif()