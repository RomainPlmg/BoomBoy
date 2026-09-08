#pragma once

#include <memory>

#include "MBC/MBC.hpp"
#include "RomFile.hpp"

namespace gb {

class Cartridge {
   public:
    enum class CreateError {
        BadMBC,
    };

    static std::expected<Cartridge, CreateError> create(RomFile&& romFile);

   private:
    Cartridge(const RomHeader& header, std::unique_ptr<cart::MBC> mbc);

    RomHeader m_header{};
    std::unique_ptr<cart::MBC> m_mbc;
};

}  // namespace gb
