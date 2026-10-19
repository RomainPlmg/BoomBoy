#pragma once

#include <cstdint>
#include <optional>

#include "Framebuffer.hpp"
#include "IORegs.hpp"

namespace gb {

class InterruptHandler;
class VRAM;

class PPU {
   public:
    enum class Mode {
        HBLANK = 0,
        VBLANK = 1,
        OAMSCAN = 2,
        DRAW = 3,
    };

    explicit PPU(VRAM& vram, InterruptHandler& interruptHandler)
        : m_vram(vram), m_interruptHandler(interruptHandler), m_fb(160, 144) {}

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

    void tick();

    [[nodiscard]] const Framebuffer& framebuffer() const { return m_fb; }
    [[nodiscard]] uint64_t frameCount() const { return m_frameCount; }

   private:
    void incrementLY();
    void renderScanline();
    void renderBGScanline();
    void renderWindowScanline();

    VRAM& m_vram;
    InterruptHandler& m_interruptHandler;
    Framebuffer m_fb;
    LCDRegs m_regs{};
    Mode m_currentMode = Mode::OAMSCAN;
    uint32_t m_dot = 0;
    uint64_t m_frameCount = 0;
};

}  // namespace gb