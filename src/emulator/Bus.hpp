#pragma once

#include <cstddef>
#include <cstdint>

namespace gb {

class BootRom;
class Cartridge;
class WRAM;
class HRAM;
class VRAM;
class OAM;
class SerialLink;
class InterruptHandler;
class Timer;
class PPU;

class Bus {
   public:
    Bus(const BootRom& bootRom, Cartridge& cartridge, WRAM& wram, HRAM& hram, VRAM& vram, OAM& oam,
        InterruptHandler& interruptHandler, Timer& timer, PPU& ppu, SerialLink& serialLink);

    [[nodiscard]] std::byte read(uint16_t addr);
    void write(uint16_t addr, std::byte val);

    void tick(uint32_t nb);      /**< T-Cycles */
    void mTick(uint32_t nb = 1); /**< M-Cycles */
    [[nodiscard]] uint64_t getTicks() const { return m_ticks; }

   private:
    const BootRom& m_bootRom;
    Cartridge& m_cartridge;
    WRAM& m_wram;
    HRAM& m_hram;
    VRAM& m_vram;
    OAM& m_oam;
    InterruptHandler& m_interruptHandler;
    Timer& m_timer;
    PPU& m_ppu;
    SerialLink& m_serialLink;

    uint64_t m_ticks = 0;
    bool m_bootRomEn = true;
};

}  // namespace gb