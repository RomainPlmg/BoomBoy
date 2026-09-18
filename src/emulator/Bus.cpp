#include "Bus.hpp"

#include "CPU.hpp"
#include "Cartridge.hpp"

namespace gb {

Bus::Bus(Cartridge& cartridge) : m_cartridge(cartridge) {}

std::byte Bus::read(uint16_t addr) {
    std::byte value{0xFF};

    if (addr <= 0x7FFFu || (addr >= 0xA000u && addr <= 0xBFFFu)) { value = m_cartridge.read(addr); }

    mTick();
    return value;
}

void Bus::write(uint16_t addr, std::byte val) {
    if (addr <= 0x7FFFu) { m_cartridge.write(addr, val); }

    mTick();
}

void Bus::tick(uint32_t nb) {
    for (size_t i = 0; i < nb; i++) {
        m_ticks++;
        // PPU.tick();
        // APU.tick();
        // etc.
    }
}

void Bus::mTick() { tick(4); }

}  // namespace gb