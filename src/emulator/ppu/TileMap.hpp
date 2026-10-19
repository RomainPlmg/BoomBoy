#pragma once

#include <cstddef>
#include <span>

#include "Tile.hpp"

namespace gb {

class VRAM;
struct LCDRegs;

class TileMap {
   public:
    [[nodiscard]] static std::span<const std::byte, 1024> background(const VRAM& vram, const LCDRegs& regs);
    [[nodiscard]] static std::span<const std::byte, 1024> window(const VRAM& vram, const LCDRegs& regs);
    [[nodiscard]] static DMGPixelColor getBackgroundPixelColor(const VRAM& vram, const LCDRegs& regs, Point coord);
    [[nodiscard]] static DMGPixelColor getWindowPixelColor(const VRAM& vram, const LCDRegs& regs, Point coord);

   private:
    static DMGPixelColor getPixelColor(const VRAM& vram, const LCDRegs& regs, std::span<const std::byte, 1024> data,
                                       Point coord);
};

}  // namespace gb