#pragma once

#include <cstdint>

#include "Registers.hpp"

namespace gb {

class Bus;

class CPU {
   public:
    CPU(Bus& bus);

    void reset();
    void step();

    [[nodiscard]] const Registers& regs() const { return m_regs; }
    [[nodiscard]] Registers& regs() { return m_regs; }
    [[nodiscard]] uint64_t cycles() const { return m_cycles; }
    [[nodiscard]] uint8_t currentOP() const { return m_currentOP; }

   private:
    Bus& m_bus;
    Registers m_regs;

    uint64_t m_cycles = 0;
    uint8_t m_currentOP = 0x00;

    [[nodiscard]] uint8_t fetch8();
    [[nodiscard]] uint16_t fetch16();
};

}  // namespace gb