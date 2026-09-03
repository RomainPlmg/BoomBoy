#include "MBC.hpp"

#include "NoMBC.hpp"

namespace gb::cart {

std::expected<std::unique_ptr<MBC>, MBC::CreateError> MBC::create(const uint8_t type, std::vector<std::byte>&& rom) {
    switch (type) {
        case 0x00: return std::make_unique<NoMBC>(std::move(rom), 0, false);
        case 0x08: return std::make_unique<NoMBC>(std::move(rom), 0x2000, false);
        case 0x09: return std::make_unique<NoMBC>(std::move(rom), 0x2000, true);
        default: return std::unexpected(CreateError::WrongCartridgeType);
    }
}

}  // namespace gb::cart