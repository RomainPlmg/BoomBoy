#include "SerialLink.hpp"

#include <iostream>
#include <utility>

namespace gb {

enum SCBit : uint8_t {
    ClockSelect = 1 << 0,
    ClockSpeed = 1 << 1,
    TransferEnable = 1 << 7,
};

void SerialLink::attachCallback(Callback cb) { m_cb = std::move(cb); }

void SerialLink::write(uint16_t addr, std::byte val) {
    const auto data = static_cast<uint8_t>(val);

    if (addr == 0XFF01) {
        m_regs.SB = data;
    } else if (addr == 0xFF02) {
        m_regs.SC = data;
    }
}

std::byte SerialLink::read(uint16_t addr) const {
    std::byte val{0xFF};

    if (addr == 0XFF01) {
        val = std::byte(m_regs.SB);
    } else if (addr == 0xFF02) {
        val = std::byte(m_regs.SC);
    }

    return val;
}

void SerialLink::tick() {
    const auto transfertActive = (m_regs.SC & SCBit::TransferEnable);
    const auto internalClock = (m_regs.SC & SCBit::ClockSelect);

    if (!transfertActive || !internalClock) return;

    if (++m_clockCounter % 512 != 0) return;
    m_clockCounter = 0;

    const auto bit = (m_regs.SB & 0x80u) >> 7;
    m_currentCharacter = (m_currentCharacter << 1) + bit;
    m_regs.SB = (m_regs.SB << 1) + 1;
    m_bitCounter++;

    if (m_bitCounter < 8) return;

    m_bitCounter = 0;
    m_regs.SC &= ~(SCBit::TransferEnable);  // Clear Tranfert Flag
    if (m_cb) m_cb(static_cast<char>(m_currentCharacter));
}

}  // namespace gb