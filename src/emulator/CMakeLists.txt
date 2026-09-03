add_library(gb_emulator STATIC
        GameBoy.cpp
)

target_include_directories(gb_emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gb_emulator PRIVATE gb_core gb_cartridge)