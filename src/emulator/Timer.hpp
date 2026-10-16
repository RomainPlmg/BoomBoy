#pragma once

#include <cstddef>
#include <cstdint>

#include "IORegs.hpp"

namespace gb {

class InterruptHandler;

class Timer {
   public:
    Timer(InterruptHandler& interruptHandler);

    void tick();
    void reset();

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

   private:
    InterruptHandler& m_interruptHandler;
    TimerRegs m_regs{};
    uint16_t m_div16Bits = 0;
};

}  // namespace gb