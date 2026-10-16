#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace gb {

class VRAM;
struct LCDRegs;

enum class DMGPixelColor {
    COLOR_0 = 0,
    COLOR_1 = 1,
    COLOR_2 = 2,
    COLOR_3 = 3,
};

struct Point {
    uint16_t x;
    uint16_t y;
};

class Tile {
   public:
    enum class AddressingMode {
        Object,
        BgWin8000,
        BGWin8800,
    };

    [[nodiscard]] static std::span<const std::byte, 16> getData(const VRAM& vram, const LCDRegs& regs, uint8_t tileId);
    [[nodiscard]] static DMGPixelColor getPixelColor(const VRAM& vram, const LCDRegs& regs, uint8_t tileId, Point coord);

   private:
    static uint16_t getAddr(const LCDRegs& regs, uint8_t tileId);
};

}  // namespace gb