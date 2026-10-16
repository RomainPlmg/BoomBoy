#include "TileMap.hpp"

#include <cassert>

#include "IORegs.hpp"
#include "RAM.hpp"

namespace gb {

std::span<const std::byte, 1024> TileMap::background(const VRAM& vram, const LCDRegs& regs) {
    uint16_t baseAddr = 0x9800;
    if (regs.LCDC & LCD_LCDC_BGTileMap_Msk) baseAddr = 0x9C00;

    const size_t offset = baseAddr - 0x8000;
    return std::span<const std::byte, 1024>{vram.m_data.data() + offset, 1024};
}

std::span<const std::byte, 1024> TileMap::window(const VRAM& vram, const LCDRegs& regs) {
    uint16_t baseAddr = 0x9800;
    if (regs.LCDC & LCD_LCDC_WinTileMap_Msk) baseAddr = 0x9C00;

    const size_t offset = baseAddr - 0x8000;
    return std::span<const std::byte, 1024>{vram.m_data.data() + offset, 1024};
}

DMGPixelColor TileMap::getBackgroundPixelColor(const VRAM& vram, const LCDRegs& regs, Point coord) {
    auto data = background(vram, regs);
    return getPixelColor(vram, regs, data, coord);
}

DMGPixelColor TileMap::getWindowColor(const VRAM& vram, const LCDRegs& regs, Point coord) {
    auto data = window(vram, regs);
    return getPixelColor(vram, regs, data, coord);
}

DMGPixelColor TileMap::getPixelColor(const VRAM& vram, const LCDRegs& regs, std::span<const std::byte, 1024> data,
                                     Point coord) {
    assert(coord.x < 256 && "Requested X coordinate is outside the tile. TileMaps are 256*256 pixels");
    assert(coord.y < 256 && "Requested Y coordinate is outside the tile. Tile are 256*256 pixels");

    Point tileCoord(coord.x / 8, coord.y / 8);
    size_t tileIdx = tileCoord.x + tileCoord.y * 32;  // 32 tiles per tilemap

    Point pixelCoord(coord.x % 8, coord.y % 8);
    return Tile::getPixelColor(vram, regs, static_cast<uint8_t>(data[tileIdx]), pixelCoord);
}

}  // namespace gb