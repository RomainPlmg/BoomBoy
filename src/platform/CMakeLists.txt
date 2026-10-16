add_library(gb_platform STATIC
    Window.cpp
)

# SDL3
FetchContent_Declare(
        sdl
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.18
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        SYSTEM
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sdl)

target_include_directories(gb_platform PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gb_platform PRIVATE SDL3::SDL3 gb_core)