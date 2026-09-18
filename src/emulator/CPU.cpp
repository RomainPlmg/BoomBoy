#include "CPU.hpp"

#include "Bus.hpp"
#include "Log.hpp"

namespace gb {

CPU::CPU(Bus& bus) : m_bus(bus), m_regs() {}

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

    LOG_INFO("CPU reseted.");
}

uint32_t CPU::step() {
    const auto prevTicks = m_bus.getTicks();
    m_currentOP = fetch8();
    execute(m_currentOP);

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

}  // namespace gb