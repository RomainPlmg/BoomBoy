add_library(gb_cartridge STATIC
        Cartridge.cpp
        RomFile.cpp
        MBC/MBC.cpp
        MBC/NoMBC.cpp
)

target_include_directories(gb_cartridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(gb_cartridge PRIVATE gb_core)