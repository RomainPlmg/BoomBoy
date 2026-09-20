#pragma once

#include <cstddef>
#include <cstdint>

namespace gb {

class Bus;

class Timer {
   public:
    struct Regs {
        uint16_t div16 = 0;
        uint8_t div = 0;
        uint8_t tima = 0;
        uint8_t tma = 0;
        uint8_t tac = 0;
    };

    Timer(Bus& bus);

    void tick();
    void reset();

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

   private:
    Bus& m_bus;
    Regs m_regs{};
};

}  // namespace gb