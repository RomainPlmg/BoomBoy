#pragma once

#include <cstddef>
#include <cstdint>

namespace gb {

class Bus;

class InterruptHandler {
   public:
    struct Regs {
        uint8_t IF;
        uint8_t IE;
    };

    static uint16_t interruptVector(uint8_t id);

    explicit InterruptHandler(Bus& bus);

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

    [[nodiscard]] bool hasPending() const;
    [[nodiscard]] uint8_t getNextPending() const;
    void clearFlag(uint8_t id);

    bool ime = false;

   private:
    Bus& m_bus;
    Regs m_regs{};
};

}  // namespace gb