#include "NoMBC.hpp"

namespace gb::cart {

std::byte NoMBC::readROM(const uint16_t addr) const {
    if (addr >= m_rom.size()) return static_cast<std::byte>(0xFFu);

    return m_rom[addr];
}

void NoMBC::writeROM(const uint16_t addr, const std::byte val) {
    (void)addr;
    (void)val;
}

std::byte NoMBC::readRAM(const uint16_t addr) const {
    if (m_ram.empty()) return static_cast<std::byte>(0xFFu);

    const auto index = addr - 0xA000;
    if (index >= m_ram.size()) return static_cast<std::byte>(0xFFu);
    return m_ram[index];
}

void NoMBC::writeRAM(const uint16_t addr, const std::byte val) {
    if (m_ram.empty()) return;

    const auto index = addr - 0xA000;
    if (index >= m_ram.size()) return;
    m_ram[index] = val;
}

}  // namespace gb::cart