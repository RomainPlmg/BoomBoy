#include "Timer.hpp"

#include "InterruptHandler.hpp"

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

Timer::Timer(InterruptHandler& interruptHandler) : m_interruptHandler(interruptHandler) {}

void Timer::tick() {
    const auto divBack = m_div16Bits;
    m_div16Bits++;
    m_regs.DIV = static_cast<uint8_t>(m_div16Bits >> 8);

    /* Check if TIMA is enabled */
    if (!(m_regs.TAC & Timer_TAC_En_Msk)) return;

    static bool overflow = false;
    if (overflow) {
        static uint8_t overflowCounter = 0;
        overflowCounter++;
        if (overflowCounter >= 16) {
            m_regs.TIMA = m_regs.TMA;
            m_interruptHandler.toggleInterrupt(InterruptSource::Timer);
            overflow = false;
            overflowCounter = 0;
            return;
        }
    }

    const auto offset = timaDivBit(m_regs.TAC & 0b11);
    const bool newBit = (m_div16Bits >> offset) & 1;

    if (const bool oldBit = (divBack >> offset) & 1;
        oldBit && !newBit) {        // Detect falling edge on the corresponding bit in DIV register
        if (m_regs.TIMA >= 0xFF) {  // Overflow
            m_regs.TIMA = m_regs.TMA;
            m_interruptHandler.toggleInterrupt(InterruptSource::Timer);
            overflow = true;
        } else {
            m_regs.TIMA++;
        }
    }
}

void Timer::reset() {}

void Timer::write(uint16_t addr, std::byte val) {
    const auto data = static_cast<uint8_t>(val);

    if (addr == 0xFF04) {  // Writing any value to this register resets it to $00
        m_regs.DIV = 0;
        m_div16Bits = 0;
    } else if (addr == 0xFF05) {
        m_regs.TIMA = data;
    } else if (addr == 0xFF06) {
        m_regs.TMA = data;
    } else if (addr == 0xFF07) {
        m_regs.TAC = data;
    }
}

std::byte Timer::read(uint16_t addr) const {
    if (addr == 0xFF04) { return std::byte(m_regs.DIV); }
    if (addr == 0xFF05) { return std::byte(m_regs.TIMA); }
    if (addr == 0xFF06) { return std::byte(m_regs.TMA); }
    if (addr == 0xFF07) { return std::byte(m_regs.TAC); }
    return std::byte(0xFF);
}

}  // namespace gb