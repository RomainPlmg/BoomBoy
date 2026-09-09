#include "Cartridge.hpp"

#include "Log.hpp"

namespace gb {

std::byte Cartridge::read(uint16_t addr) const {
    if (addr <= 0x7FFF) return m_mbc->readROM(addr);
    else return m_mbc->readRAM(addr);
}

void Cartridge::write(uint16_t addr, std::byte val) {
    if (addr <= 0x7FFF) m_mbc->writeROM(addr, val);
    else m_mbc->writeRAM(addr, val);
}

std::expected<Cartridge, Cartridge::CreateError> Cartridge::create(RomFile&& romFile) {
    const auto header = romFile.header();

    // Build the MBC
    const auto mbcType = header.type;
    const auto romSize = (32 << header.romSize) * 1024;
    const auto ramSize = cart::RAM_CAPACITY.at(header.ramSize) * 0x400;

    auto mbcExp = cart::MBC::create(mbcType, std::move(romFile).data(), romSize, ramSize);

    if (!mbcExp.has_value()) {
        LOG_ERROR("Failed to build the MBC. Unknown MBC type 0x{:02X}", mbcType);
        return std::unexpected(CreateError::BadMBC);
    }

    auto mbc = std::move(mbcExp.value());

    return Cartridge(header, std::move(mbc));
}

Cartridge::Cartridge(const RomHeader& header, std::unique_ptr<cart::MBC> mbc)
    : m_header(header), m_mbc(std::move(mbc)) {}

}  // namespace gb