#include "Bus.hpp"

#include "CPU.hpp"
#include "Cartridge.hpp"

namespace gb {

Bus::Bus(Cartridge& cartridge) : m_cartridge(cartridge) {}

std::byte Bus::read(uint16_t addr) const {
    if (addr <= 0x7FFFu || (addr >= 0xA000u && addr <= 0xBFFFu)) {
        return m_cartridge.read(addr);
    } else {
        return std::byte{0xFF};
    }
}

void Bus::write(uint16_t addr, std::byte val) {
    if (addr <= 0x7FFFu) { m_cartridge.write(addr, val); }
}

}  // namespace gb