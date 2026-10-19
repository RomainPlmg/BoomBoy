#pragma once

#include <cstdint>

#include "BootRom.hpp"
#include "Bus.hpp"
#include "CPU.hpp"
#include "Cartridge.hpp"
#include "RAM.hpp"
#include "SerialLink.hpp"
#include "Timer.hpp"
#include "ppu/PPU.hpp"

namespace gb {

constexpr uint32_t CPU_FREQ = 4194304;
constexpr uint32_t CYCLES_PER_FRAME = 70224;

class GameBoy {
   public:
    explicit GameBoy(Cartridge&& cartridge, BootRom&& bootRom);

    uint32_t step();

    [[nodiscard]] const Cartridge& cartridge() const { return m_cartridge; }
    [[nodiscard]] SerialLink& serialLink() { return m_serialLink; }
    [[nodiscard]] InterruptHandler& interruptHandler() { return m_interruptHandler; }
    [[nodiscard]] const CPU& cpu() const { return m_cpu; }
    [[nodiscard]] const PPU& ppu() const { return m_ppu; }

   private:
    BootRom m_bootRom;
    Cartridge m_cartridge;
    SerialLink m_serialLink;
    WRAM m_wram;
    HRAM m_hram;
    VRAM m_vram;
    OAM m_oam;
    Bus m_bus;
    InterruptHandler m_interruptHandler;
    Timer m_timer;
    PPU m_ppu;
    CPU m_cpu;
};

}  // namespace gb