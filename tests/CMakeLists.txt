include(FetchContent)

# ============================================================================
# Add Mooneye tests
# ============================================================================

add_executable(MooneyeRunner
        MooneyeRunner.cpp
)
target_include_directories(MooneyeRunner PRIVATE ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(MooneyeRunner PRIVATE gb_core gb_cartridge gb_emulator)

FetchContent_Declare(
        mooneye
        URL file://C:/Users/X55808/Downloads/mts-20211031-1956-1ae00bf.tar.gz
        URL_HASH SHA256=778866E78A174EE81CB4671DE501FA4DDAC6A91AE248D913C4694520CE501D0A
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(mooneye)
set(MOONEYE_ROOT "${mooneye_SOURCE_DIR}")

file(GLOB_RECURSE MOONEYE_ROMS
        "${MOONEYE_ROOT}/acceptance/*.gb"
        "${MOONEYE_ROOT}/emulator-only/*.gb"
)
list(FILTER MOONEYE_ROMS EXCLUDE REGEX "-(C|A|S|cgb|agb|sgb)\\.gb$")

foreach(rom IN LISTS MOONEYE_ROMS)
    file(RELATIVE_PATH rel "${MOONEYE_ROOT}" "${rom}")
    string(REGEX REPLACE "\\.gb$" "" rel "${rel}")
    string(REPLACE "/" "." test_name "${rel}")

    message(STATUS "Add test mooneye.${test_name}")

    add_test(NAME "mooneye.${test_name}"
            COMMAND MooneyeRunner "${rom}")
    set_tests_properties("mooneye.${test_name}" PROPERTIES
            TIMEOUT 10
            LABELS "mooneye")
endforeach()

# ============================================================================
# Add Blargg tests
# ============================================================================

add_executable(BlarggRunner
        BlarggRunner.cpp
)
target_include_directories(BlarggRunner PRIVATE ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(BlarggRunner PRIVATE gb_core gb_cartridge gb_emulator)

FetchContent_Declare(
        blargg
        GIT_REPOSITORY https://github.com/retrio/gb-test-roms.git
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(blargg)
set(BLARGG_ROOT "${blargg_SOURCE_DIR}")

file(GLOB_RECURSE BLARGG_ROMS
        "${BLARGG_ROOT}/**/*gb"
)
list(FILTER BLARGG_ROMS EXCLUDE REGEX "/cgb_sound/")

foreach(rom IN LISTS BLARGG_ROMS)
    file(RELATIVE_PATH rel "${BLARGG_ROOT}" "${rom}")
    string(REGEX REPLACE "\\.gb$" "" rel "${rel}")
    string(REPLACE "/" "." test_name "${rel}")

    message(STATUS "Add test blargg.${test_name}")

    add_test(NAME "blargg.${test_name}"
            COMMAND BlarggRunner "${rom}")
    set_tests_properties("blargg.${test_name}" PROPERTIES
            TIMEOUT 10
            LABELS "blargg")
endforeach()