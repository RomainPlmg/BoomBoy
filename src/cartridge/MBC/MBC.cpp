#include "MBC.hpp"

#include "MBC1.hpp"
#include "NoMBC.hpp"

namespace gb::cart {

std::expected<std::unique_ptr<MBC>, MBC::CreateError> MBC::create(const uint8_t type, std::vector<std::byte>&& rom,
                                                                  const size_t romSize, const size_t ramSize) {
    switch (type) {
        case 0x00: return std::make_unique<NoMBC>(std::move(rom), romSize, false);
        case 0x01: return std::make_unique<MBC1>(std::move(rom), romSize, 0, false);
        case 0x02: return std::make_unique<MBC1>(std::move(rom), romSize, ramSize, false);
        case 0x03: return std::make_unique<MBC1>(std::move(rom), romSize, ramSize, true);
        case 0x08: return std::make_unique<NoMBC>(std::move(rom), ramSize, false);
        case 0x09: return std::make_unique<NoMBC>(std::move(rom), ramSize, true);
        default: return std::unexpected(CreateError::WrongCartridgeType);
    }
}

}  // namespace gb::cart