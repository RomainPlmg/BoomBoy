#pragma once

#include <cstdint>

#include "Bus.hpp"
#include "CPU.hpp"
#include "Cartridge.hpp"

namespace gb {

class Cartridge;

class GameBoy {
   public:
    explicit GameBoy(Cartridge&& cartridge);

    uint64_t step();

    [[nodiscard]] const Cartridge& cartridge() const { return m_cartridge; }
    [[nodiscard]] const CPU& cpu() const { return m_cpu; }

   private:
    Cartridge m_cartridge;
    Bus m_bus;
    CPU m_cpu;
};

}  // namespace gb