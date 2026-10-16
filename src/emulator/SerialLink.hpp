#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

#include "IORegs.hpp"

namespace gb {

class InterruptHandler;

class SerialLink {
   public:
    SerialLink(InterruptHandler& interruptHandler) : m_interruptHandler(interruptHandler) {}
    using Callback = std::function<void(char c)>;

    void attachCallback(Callback cb);

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

    void tick();

   private:
    InterruptHandler& m_interruptHandler;
    Callback m_cb;
    SerialLinkRegs m_regs{};
    uint32_t m_clockCounter = 0;
    uint8_t m_bitCounter = 0;
    uint8_t m_currentCharacter = 0;
};

}  // namespace gb