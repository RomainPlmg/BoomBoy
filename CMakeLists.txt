cmake_minimum_required(VERSION 3.30.0)

project(BoomBoy
        VERSION 0.1.0
        DESCRIPTION "A tiny Game Boy emulator!"
        LANGUAGES C CXX)

include(FetchContent)

# ============================================================================
# Options
# ============================================================================
option(BB_DOWNLOAD_TEST_ROMS "Download Mooneye test ROMs" OFF)

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

target_link_libraries(BoomBoy PRIVATE gb_core gb_cartridge gb_emulator)

# ============================================================================
# Tests
# ============================================================================
if(GB_DOWNLOAD_TEST_ROMS)
    FetchContent_Declare(
            mooneye
            URL https://gekkio.fi/files/mooneye-test-suite/mts-20260714-0944-31510e1/mts-20260714-0944-31510e1.tar.gz
            URL_HASH SHA256=5F0694E4E9E58441DB23FF057A5AB681870D4CFAED91267E99885936DB8E6F1D
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(mooneye)
    set(MOONEYE_ROOT "${mooneye_SOURCE_DIR}")

    file(GLOB_RECURSE MOONEYE_ROMS
            "${MOONEYE_ROOT}/acceptance/*.gb"
            "${MOONEYE_ROOT}/emulator-only/*.gb"
    )
    list(FILTER MOONEYE_ROMS EXCLUDE REGEX "-(C|A|S|cgb|agb|sgb)\\.gb$")

    # Run tests
    foreach(rom IN LISTS MOONEYE_ROMS)
        file(RELATIVE_PATH rel "${MOONEYE_ROOT}" "${rom}")
        string(REGEX REPLACE "\\.gb$" "" rel "${rel}")
        string(REPLACE "/" "." test_name "${rel}")

        message(STATUS "Add test mooneye.${test_name}")

        add_test(NAME "mooneye.${test_name}"
                COMMAND BoomBoy "${rom}")
        set_tests_properties("mooneye.${test_name}" PROPERTIES
                TIMEOUT 10
                LABELS "mooneye")
    endforeach()

    enable_testing()
endif()