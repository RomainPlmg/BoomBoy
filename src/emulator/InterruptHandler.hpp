#pragma once

#include <cstddef>
#include <cstdint>

#include "IORegs.hpp"

namespace gb {

class Bus;

enum class InterruptSource : uint8_t {
    VBLANK = 0,
    LCD,
    Timer,
    Serial,
    Joypad,
};

class InterruptHandler {
   public:
    static uint16_t interruptVector(uint8_t id);

    explicit InterruptHandler(Bus& bus);

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

    void toggleInterrupt(InterruptSource source);
    [[nodiscard]] bool hasPending() const;
    [[nodiscard]] uint8_t getNextPending() const;
    void clearFlag(uint8_t id);

    bool ime = false;

   private:
    Bus& m_bus;
    InterruptRegs m_regs{};
};

}  // namespace gb