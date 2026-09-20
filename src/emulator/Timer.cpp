#include "Timer.hpp"

#include "Bus.hpp"

namespace gb {

static uint8_t timaDivBit(uint8_t key) {
    switch (key) {
        case 0: return 9;
        case 1: return 3;
        case 2: return 5;
        case 3: return 7;
        default: return 1;
    }
}

Timer::Timer(Bus& bus) : m_bus(bus) {}

void Timer::tick() {
    const auto divBack = m_regs.div16;
    m_regs.div16++;
    m_regs.div = static_cast<uint8_t>(m_regs.div16 >> 8);

    /* Check if TIMA is enabled */
    if (!(m_regs.tac & 0b100)) return;

    static bool overflow = false;
    if (overflow) {
        static uint8_t overflowCounter = 0;
        overflowCounter++;
        if (overflowCounter >= 16) {
            m_regs.tima = m_regs.tma;
            const auto intf = m_bus.read(0xFF0F);
            m_bus.write(0xFF0F, intf | std::byte(1 << 2));
            overflow = false;
            overflowCounter = 0;
            return;
        }
    }

    const auto offset = timaDivBit(m_regs.tac & 0b11);
    const bool newBit = (m_regs.div16 >> offset) & 1;

    if (const bool oldBit = (divBack >> offset) & 1;
        oldBit && !newBit) {        // Detect falling edge on the corresponding bit in DIV register
        if (m_regs.tima >= 0xFF) {  // Overflow
            m_regs.tima = m_regs.tma;
            const auto intf = m_bus.read(0xFF0F);
            m_bus.write(0xFF0F, intf | std::byte(1 << 2));
            overflow = true;
        } else {
            m_regs.tima++;
        }
    }
}

void Timer::reset() {}

void Timer::write(uint16_t addr, std::byte val) {
    const auto data = static_cast<uint8_t>(val);

    if (addr == 0xFF04) {
        m_regs.div = data;
    } else if (addr == 0xFF05) {
        m_regs.tima = data;
    } else if (addr == 0xFF06) {
        m_regs.tma = data;
    } else if (addr == 0xFF07) {
        m_regs.tac = data;
    }
}

std::byte Timer::read(uint16_t addr) const {
    if (addr == 0xFF04) { return std::byte(m_regs.div); }
    if (addr == 0xFF05) { return std::byte(m_regs.tima); }
    if (addr == 0xFF06) { return std::byte(m_regs.tma); }
    if (addr == 0xFF07) { return std::byte(m_regs.tac); }
    return std::byte(0xFF);
}

}  // namespace gb