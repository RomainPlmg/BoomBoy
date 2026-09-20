add_library(gb_emulator STATIC
        Bus.cpp
        CPU.cpp
        GameBoy.cpp
        Instructions.cpp
        IntructionsCB.cpp
        RAM.cpp
        SerialLink.cpp
        InterruptHandler.cpp
        Timer.cpp
)

target_include_directories(gb_emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gb_emulator PRIVATE gb_core gb_cartridge)