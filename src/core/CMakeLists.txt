add_library(gb_core STATIC
        Log.cpp
        Chrono.cpp
)

target_include_directories(gb_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

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

target_link_libraries(gb_core PRIVATE spdlog::spdlog)