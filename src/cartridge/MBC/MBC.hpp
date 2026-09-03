#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <memory>
#include <vector>

namespace gb::cart {

class MBC {
   public:
    enum class CreateError {
        WrongCartridgeType,
    };

    virtual ~MBC() = default;

    [[nodiscard]] virtual std::byte readROM(uint16_t addr) const = 0;
    virtual void writeROM(uint16_t addr, std::byte val) = 0;

    [[nodiscard]] virtual std::byte readRAM(uint16_t addr) const = 0;
    virtual void writeRAM(uint16_t addr, std::byte val) = 0;

    // Factory method
    static std::expected<std::unique_ptr<MBC>, CreateError> create(uint8_t type, std::vector<std::byte>&& rom);
};

}  // namespace gb::cart
