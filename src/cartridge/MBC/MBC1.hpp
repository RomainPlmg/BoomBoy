#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "MBC.hpp"

namespace gb::cart {

class MBC1 : public MBC {
   public:
    MBC1(std::vector<std::byte>&& rom, size_t romSize, size_t ramSize, bool hasBattery);

    [[nodiscard]] std::byte readROM(uint16_t addr) const override;
    void writeROM(uint16_t addr, std::byte val) override;

    [[nodiscard]] std::byte readRAM(uint16_t addr) const override;
    void writeRAM(uint16_t addr, std::byte val) override;

   private:
    const uint8_t m_romBankMask;  // 5 bits to address ROM Banks, may be less (256 KiB -> 16 Banks -> 4 bits)

    bool m_ramEn = false;
    uint8_t m_romBankNb = 0;
    uint8_t m_ramBankNb = 0;
    uint8_t m_bankingModeSel = 0;

    std::vector<std::byte> m_rom;
    std::vector<std::byte> m_ram;
    bool m_hasBattery;

    [[nodiscard]] uint8_t currentRomBankLow() const;
    [[nodiscard]] uint8_t currentRomBankHigh() const;
    [[nodiscard]] uint8_t currentRamBank() const;
};

}  // namespace gb::cart
