#include "Tile.hpp"
#include <cassert>

#include "../RAM.hpp"
#include "IORegs.hpp"

namespace gb {

std::span<const std::byte, 16> Tile::getData(const VRAM& vram, const LCDRegs& regs, uint8_t tileId) {
    const auto offset = static_cast<std::size_t>(getAddr(regs, tileId) - 0x8000);
    return std::span<const std::byte, 16>{vram.m_data.data() + offset, 16};
}

DMGPixelColor Tile::getPixelColor(const VRAM& vram, const LCDRegs& regs, uint8_t tileId, Point coord) {
    assert(coord.x < 8 && "Requested X coordinate is outside the tile. Tile are 8*8 pixels");
    assert(coord.y < 8 && "Requested Y coordinate is outside the tile. Tile are 8*8 pixels");

    auto data = getData(vram, regs, tileId);
    size_t lineIdx = 2 * coord.y; // Recover the corresponding line of data

    // Combine the 2 bytes into 1 uint16_t. See https://gbdev.io/pandocs/Tile_Data.html#data-format
    uint16_t result = 0;
    auto a = static_cast<uint8_t>(data[lineIdx]);
    auto b = static_cast<uint8_t>(data[lineIdx + 1]);

    for (int i = 0; i < 8; ++i) {
        result |= ((a >> i) & 1u) << (2 * i);
        result |= ((b >> i) & 1u) << (2 * i + 1);
    }

    // Each line store 8 pixel colors, use a mask to isolate the 2 utils bits
    auto color = (result & (0b11 << (14 - (coord.x * 2)))) >> (14 - (coord.x * 2));
    return static_cast<DMGPixelColor>(color);
}

uint16_t Tile::getAddr(const LCDRegs& regs, uint8_t tileId) {
    if (!(regs.LCDC & LCD_LCDC_BGWTiles_Msk)) { return 0x9000 + static_cast<int8_t>(tileId) * 16; }
    return 0x8000 + tileId * 16;
}

}  // namespace gb