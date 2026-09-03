#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "MBC.hpp"

namespace gb::cart {

class NoMBC : public MBC {
   public:
    explicit NoMBC(std::vector<std::byte>&& rom, const size_t ramSize, const bool hasBattery)
        : m_rom(std::move(rom)), m_ram(ramSize), m_hasBattery(hasBattery) {}

    [[nodiscard]] std::byte readROM(uint16_t addr) const override;
    void writeROM(uint16_t addr, std::byte val) override;

    [[nodiscard]] std::byte readRAM(uint16_t addr) const override;
    void writeRAM(uint16_t addr, std::byte val) override;

   private:
    std::vector<std::byte> m_rom;  // 32 KiB of ROM
    std::vector<std::byte> m_ram;  // 8 KiB of RAM (Optional)
    bool m_hasBattery;
};

}  // namespace gb::cart
