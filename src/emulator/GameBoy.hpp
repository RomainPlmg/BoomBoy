#pragma once

#include <cstdint>

#include "Bus.hpp"
#include "CPU.hpp"
#include "Cartridge.hpp"
#include "RAM.hpp"
#include "SerialLink.hpp"

namespace gb {

class Cartridge;

class GameBoy {
   public:
    explicit GameBoy(Cartridge&& cartridge);

    uint32_t step();

    [[nodiscard]] const Cartridge& cartridge() const { return m_cartridge; }
    [[nodiscard]] SerialLink& serialLink() { return m_serialLink; }
    [[nodiscard]] InterruptHandler& interruptHandler() { return m_interruptHandler; }
    [[nodiscard]] const CPU& cpu() const { return m_cpu; }

   private:
    Cartridge m_cartridge;
    SerialLink m_serialLink;
    WRAM m_wram;
    HRAM m_hram;
    VRAM m_vram;
    OAM m_oam;
    Bus m_bus;
    InterruptHandler m_interruptHandler;
    CPU m_cpu;
};

}  // namespace gb