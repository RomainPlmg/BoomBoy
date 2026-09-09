#pragma once

#include <cstddef>
#include <cstdint>

namespace gb {

class CPU;
class Cartridge;

class Bus {
   public:
    Bus(Cartridge& cartridge);

    [[nodiscard]] std::byte read(uint16_t addr) const;
    void write(uint16_t addr, std::byte val);

   private:
    Cartridge& m_cartridge;
};

}  // namespace gb