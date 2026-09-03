#pragma once

#include <expected>
#include <filesystem>
#include <string_view>
#include <vector>

#include "CartridgeInfo.hpp"

namespace gb {

struct RomHeader {
    uint8_t entry[4];
    uint8_t logo[48];
    uint8_t title[16];
    uint16_t newLicenseeCode;
    uint8_t sgb;
    uint8_t type;
    uint8_t romSize;
    uint8_t ramSize;
    uint8_t destCode;
    uint8_t licenseeCode;
    uint8_t version;
    uint8_t checksum;
    uint16_t globalChecksum;

    [[nodiscard]] std::string_view titleStr() const {
        std::string_view sv{reinterpret_cast<const char*>(title), sizeof(title)};
        return sv.substr(0, sv.find('\0'));
    }

    [[nodiscard]] std::string_view typeStr() const {
        if (type < cart::CARTRIDGE_TYPES.size()) return cart::CARTRIDGE_TYPES.at(type);
        return "Unknown";
    };

    [[nodiscard]] std::string ramSizeStr() const {
        if (ramSize < cart::RAM_CAPACITY.size()) return std::to_string(cart::RAM_CAPACITY.at(ramSize)) + " KiB";
        return "Unknown";
    }
};

class RomFile {
   public:
    enum class LoadError {
        FileNotFound,
        FileInvalid,
        FileCorrupted,
    };

    static std::expected<RomFile, LoadError> load(const std::filesystem::path& path);

    [[nodiscard]] const RomHeader& header() const { return m_header; }
    [[nodiscard]] std::vector<std::byte>&& data() && { return std::move(m_data); }
    [[nodiscard]] const std::vector<std::byte>& data() const& { return m_data; }

   private:
    RomFile() = default;

    std::vector<std::byte> m_data;
    RomHeader m_header{};
};

}  // namespace gb