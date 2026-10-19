#pragma once

#include <expected>
#include <filesystem>

namespace gb {

class BootRom {
   public:
    enum class LoadError {
        FileNotFound,
        FileInvalid,
        FileCorrupted,
    };

    static std::expected<BootRom, LoadError> load(const std::filesystem::path& path);
    [[nodiscard]] const std::vector<std::byte>& data() const& { return m_data; }

   private:
    std::vector<std::byte> m_data;
};

}  // namespace gb