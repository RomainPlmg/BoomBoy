#include "Cartridge.hpp"

#include "Log.hpp"

namespace gb {

std::expected<Cartridge, Cartridge::CreateError> Cartridge::create(RomFile&& romFile) {
    Cartridge cartridge;

    cartridge.m_header = romFile.header();

    // Build the MBC
    const auto mbcType = cartridge.m_header.type;
    auto mbcExp = cart::MBC::create(mbcType, std::move(romFile).data());

    if (!mbcExp.has_value()) {
        LOG_ERROR("Failed to build the MBC. Unknown MBC type 0x{:02X}", mbcType);
        return std::unexpected(CreateError::BadMBC);
    }

    cartridge.m_mbc = std::move(mbcExp.value());

    return cartridge;
}

}  // namespace gb