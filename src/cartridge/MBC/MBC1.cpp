#include "MBC1.hpp"

#include <cassert>

namespace gb::cart {

MBC1::MBC1(std::vector<std::byte>&& rom, const size_t romSize, const size_t ramSize, const bool hasBattery)
    : m_romBankMask((romSize / ROM_BANK_SIZE) - 1), m_rom(std::move(rom)), m_ram(ramSize), m_hasBattery(hasBattery) {
    assert((romSize !=0) && (romSize % 2 == 0) && "Invalid size of ROM");
}

std::byte MBC1::readROM(const uint16_t addr) const {
    if (addr <= 0x3FFFu) { return m_rom[ROM_BANK_SIZE * currentRomBankLow() + addr]; }
    if (addr <= 0x7FFFu) { return m_rom[ROM_BANK_SIZE * currentRomBankHigh() + (addr - 0x4000u)]; }
    return std::byte{0xFFu};
}

void MBC1::writeROM(const uint16_t addr, const std::byte val) {
    const auto data = static_cast<uint8_t>(val);

    if (addr <= 0x1FFFu) {
        m_ramEn = (data & 0x0Fu) == 0x0Au;
    } else if (addr <= 0x3FFFu) {
        m_romBankNb = data & 0x1Fu;
    } else if (addr <= 0x5FFFu) {
        m_ramBankNb = data & 0x3u;
    } else if (addr <= 0x7FFFu) {
        m_bankingModeSel = data & 0x1u;
    }
}

std::byte MBC1::readRAM(const uint16_t addr) const {
    if (!m_ramEn || m_ram.empty()) return std::byte{0xFF};
    const size_t index = RAM_BANK_SIZE * currentRamBank() + (addr - 0xA000);
    return m_ram[index % m_ram.size()];
}

void MBC1::writeRAM(const uint16_t addr, const std::byte val) {
    if (!m_ramEn || m_ram.empty()) return;
    const size_t index = RAM_BANK_SIZE * currentRamBank() + (addr - 0xA000);
    if (index >= m_ram.size()) return;
    m_ram[index] = val;
}

uint8_t MBC1::currentRomBankLow() const {
    if (m_bankingModeSel == 0) return 0;
    return (m_ramBankNb << 5) & m_romBankMask;
}

uint8_t MBC1::currentRomBankHigh() const {
    auto bank = m_romBankNb;
    if (bank == 0) bank = 1;  // Cannot apply the value 0 to the ROM bank
    bank |= (m_ramBankNb << 5);
    return bank & m_romBankMask;
}

uint8_t MBC1::currentRamBank() const {
    if (m_bankingModeSel == 0) return 0;
    return m_ramBankNb;
}

}  // namespace gb::cart