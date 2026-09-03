#include "RomFile.hpp"

#include <fstream>

#include "ByteReader.hpp"
#include "Log.hpp"

namespace gb {

std::expected<RomFile, RomFile::LoadError> RomFile::load(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) {
        LOG_ERROR("'{}': File not found.", path.string());
        return std::unexpected(LoadError::FileNotFound);
    }

    // Recover the file size
    const std::streamoff fileSize = file.tellg();
    if (static_cast<size_t>(fileSize) < 0x0150) {
        LOG_ERROR("'{}': File is too small to be a valid GameBoy ROM.", path.string());
        return std::unexpected(LoadError::FileInvalid);
    }
    file.seekg(0);

    RomFile cartridge;
    cartridge.m_data.resize(fileSize);

    if (!file.read(reinterpret_cast<char*>(cartridge.m_data.data()),
                   static_cast<std::streamsize>(cartridge.m_data.size()))) {
        LOG_ERROR("'{}': Unable to read file.", path.string());
        return std::unexpected(LoadError::FileCorrupted);
    }

    // Fill the cartridge header
    readArray(cartridge.m_data, 0x0100, cartridge.m_header.entry);
    readArray(cartridge.m_data, 0x0104, cartridge.m_header.logo);
    readArray(cartridge.m_data, 0x0134, cartridge.m_header.title);
    cartridge.m_header.newLicenseeCode = readU16BE(cartridge.m_data, 0x0144);
    cartridge.m_header.sgb = readU8(cartridge.m_data, 0x0146);
    cartridge.m_header.type = readU8(cartridge.m_data, 0x0147);
    cartridge.m_header.romSize = readU8(cartridge.m_data, 0x0148);
    cartridge.m_header.ramSize = readU8(cartridge.m_data, 0x0149);
    cartridge.m_header.destCode = readU8(cartridge.m_data, 0x014A);
    cartridge.m_header.licenseeCode = readU8(cartridge.m_data, 0x014B);
    cartridge.m_header.version = readU8(cartridge.m_data, 0x014C);
    cartridge.m_header.checksum = readU8(cartridge.m_data, 0x014D);
    cartridge.m_header.globalChecksum = readU16BE(cartridge.m_data, 0x014E);

    // Checksum verification, see https://gbdev.io/pandocs/The_Cartridge_Header.html#014d--header-checksum
    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - readU8(cartridge.m_data, address) - 1;
    }

    if (checksum != cartridge.m_header.checksum) {
        LOG_WARN("Header checksum mismatch! Calculated: 0x{:02X}, Header: 0x{:02X}", checksum,
                 cartridge.m_header.checksum);
    }

    LOG_INFO("ROM file loaded:");
    LOG_INFO("\tTitle:        {}", cartridge.m_header.titleStr());
    LOG_INFO("\tType:         {}", cartridge.m_header.typeStr());
    LOG_INFO("\tROM Size:     {} KiB", 32 << cartridge.m_header.romSize);
    LOG_INFO("\tRAM Size:     {}", cartridge.m_header.ramSizeStr());
    LOG_INFO("\tROM Version:  {}", cartridge.m_header.version);
    LOG_INFO("\tChecksum:     0x{:02X} ({})", cartridge.m_header.checksum,
             (checksum == cartridge.m_header.checksum) ? "PASS" : "FAIL");

    return cartridge;
}

}  // namespace gb