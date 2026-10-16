#include "InterruptHandler.hpp"

#include "Log.hpp"

namespace gb {

uint16_t InterruptHandler::interruptVector(uint8_t id) {
    switch (id) {
        case 0: return 0x0040;
        case 1: return 0x0048;
        case 2: return 0x0050;
        case 3: return 0x0058;
        case 4: return 0x0060;
        default: return 0x0000;
    }
}

InterruptHandler::InterruptHandler(Bus& bus) : m_bus(bus) {}

void InterruptHandler::write(uint16_t addr, std::byte val) {
    if (addr == 0xFF0Fu) { m_regs.IF = static_cast<uint8_t>(val); }
    if (addr == 0xFFFFu) { m_regs.IE = static_cast<uint8_t>(val); }
}

std::byte InterruptHandler::read(uint16_t addr) const {
    if (addr == 0xFF0Fu) { return std::byte(m_regs.IF); }
    if (addr == 0xFFFFu) { return std::byte(m_regs.IE); }

    return std::byte(0xFF);
}

void InterruptHandler::toggleInterrupt(InterruptSource source) { m_regs.IF |= (1 << static_cast<uint8_t>(source)); }

bool InterruptHandler::hasPending() const { return (m_regs.IE & m_regs.IF & 0x1F) != 0; }

uint8_t InterruptHandler::getNextPending() const {
    const uint8_t pending = m_regs.IE & m_regs.IF & 0x1F;  // Mask for enabled pending instructions
    for (uint8_t id = 0; id <= 4; id++) {
        if (pending & (1 << id)) { return id; }
    }
    return 0xFF;
}

void InterruptHandler::clearFlag(uint8_t id) {
    if (id > 4) {
        LOG_ERROR("{} is an invalid interrupt ID", id);
        return;
    }
    m_regs.IF &= ~(1 << id);
}

}  // namespace gb