#pragma once

#include <cstddef>
#include <cstdint>

namespace gb {

class CPU;
class Cartridge;

class Bus {
   public:
    Bus(Cartridge& cartridge);

    [[nodiscard]] std::byte read(uint16_t addr);
    void write(uint16_t addr, std::byte val);

    void tick(uint32_t nb); /**< T-Cycles */
    void mTick();           /**< M-Cycles */
    [[nodiscard]] uint64_t getTicks() const { return m_ticks; }

   private:
    Cartridge& m_cartridge;
    uint64_t m_ticks = 0;
};

}  // namespace gb