#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace gb {

class SerialLink {
   public:
    struct Regs {
        uint8_t SB;
        uint8_t SC;
    };

    using Callback = std::function<void(char c)>;

    void attachCallback(Callback cb);

    void write(uint16_t addr, std::byte val);
    [[nodiscard]] std::byte read(uint16_t addr) const;

    void tick();

   private:
    Callback m_cb;
    Regs m_regs{};
    uint32_t m_clockCounter = 0;
    uint8_t m_bitCounter = 0;
    uint8_t m_currentCharacter = 0;
};

}  // namespace gb