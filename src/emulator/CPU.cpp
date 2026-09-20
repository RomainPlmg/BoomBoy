#include "CPU.hpp"

#include <iostream>

#include "Bus.hpp"
#include "Log.hpp"

namespace gb {

CPU::CPU(Bus& bus, InterruptHandler& interruptHandler) : m_bus(bus), m_regs(), m_interruptHandler(interruptHandler) {}

void CPU::reset() {
    m_regs.a = 0x01;
    m_regs.f = 0xB0;
    m_regs.b = 0x00;
    m_regs.c = 0x13;
    m_regs.d = 0x00;
    m_regs.e = 0xD8;
    m_regs.h = 0x01;
    m_regs.l = 0x4D;
    m_regs.sp = 0xFFFE;
    m_regs.pc = 0x0100;

    m_interruptHandler.ime = false;
    m_requestIMEEn = false;
    m_halted = false;

    LOG_INFO("CPU reseted.");
}

uint32_t CPU::step() {
    const auto prevTicks = m_bus.getTicks();

    if (m_halted) {
        if (m_interruptHandler.hasPending()) {
            m_halted = false;
            if (!m_interruptHandler.ime) {
                m_haltBug = true;
                m_bus.mTick();
                return m_bus.getTicks() - prevTicks;
            }
        } else {
            m_bus.mTick();
            return m_bus.getTicks() - prevTicks;
        }
    }

    if (!handleInterrupts()) {
        m_currentOP = fetch8();

        execute(m_currentOP);

        if (m_requestIMEEn) {
            m_interruptHandler.ime = true;
            m_requestIMEEn = false;
        }
    }

    return static_cast<uint32_t>(m_bus.getTicks() - prevTicks);
}

uint8_t CPU::fetch8() {
    const auto val = static_cast<uint8_t>(m_bus.read(m_regs.pc++));
    return val;
}

uint16_t CPU::fetch16() {
    const auto low = fetch8();
    const auto high = fetch8();

    return (high << 8) | low;
}

uint8_t CPU::pop8() { return static_cast<uint8_t>(m_bus.read(m_regs.sp++)); }

uint16_t CPU::pop16() {
    const uint8_t low = pop8();
    const uint8_t high = pop8();

    return (high << 8) | low;
}

void CPU::push8(uint8_t val) { m_bus.write(--m_regs.sp, std::byte(val)); }

void CPU::push16(uint16_t val) {
    const uint8_t high = (val >> 8) & 0xFF;
    const uint8_t low = val & 0xFF;

    m_bus.write(--m_regs.sp, std::byte(high));
    m_bus.write(--m_regs.sp, std::byte(low));
}

bool CPU::handleInterrupts() {
    if (m_interruptHandler.ime && m_interruptHandler.hasPending()) {
        const uint8_t id = m_interruptHandler.getNextPending();
        m_bus.mTick(2);
        if (id != 0xFF) {
            m_interruptHandler.ime = false;
            push16(m_regs.pc);
            m_interruptHandler.clearFlag(id);
            m_regs.pc = InterruptHandler::interruptVector(id);
            m_bus.mTick();
        }
        return true;
    }
    return false;
}

}  // namespace gb