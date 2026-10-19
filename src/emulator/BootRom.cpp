#include "BootRom.hpp"

#include <fstream>

#include "Log.hpp"

namespace gb {

std::expected<BootRom, BootRom::LoadError> BootRom::load(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) {
        LOG_ERROR("'{}': File not found.", path.string());
        return std::unexpected(LoadError::FileNotFound);
    }

    // Recover the file size
    const std::streamoff fileSize = file.tellg();
    if (static_cast<size_t>(fileSize) < 0x0100) {
        LOG_ERROR("'{}': The boot ROM must be exactly 256 bytes long. It is currently {} bytes long", path.string(),
                  fileSize);
        return std::unexpected(LoadError::FileInvalid);
    }
    file.seekg(0);

    BootRom rom;
    rom.m_data.resize(fileSize);

    if (!file.read(reinterpret_cast<char*>(rom.m_data.data()), static_cast<std::streamsize>(rom.m_data.size()))) {
        LOG_ERROR("'{}': Unable to read file.", path.string());
        return std::unexpected(LoadError::FileCorrupted);
    }

    return rom;
}

}  // namespace gb