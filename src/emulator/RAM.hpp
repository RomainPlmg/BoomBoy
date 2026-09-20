#pragma once

#include <cstddef>
#include <vector>

namespace gb {

class WRAM {
   public:
    WRAM() : m_data(0x2000u) {}

    [[nodiscard]] std::byte read(uint16_t addr) const;
    void write(uint16_t addr, std::byte val);

   private:
    std::vector<std::byte> m_data;
};

class HRAM {
   public:
    HRAM() : m_data(0x007F) {}

    [[nodiscard]] std::byte read(uint16_t addr) const;
    void write(uint16_t addr, std::byte val);

   private:
    std::vector<std::byte> m_data;
};

class VRAM {
   public:
    VRAM() : m_data(0x2000) {}

    [[nodiscard]] std::byte read(uint16_t addr) const;
    void write(uint16_t addr, std::byte val);

   private:
    std::vector<std::byte> m_data;
};

class OAM {
   public:
    OAM() : m_data(0x00A0) {}

    [[nodiscard]] std::byte read(uint16_t addr) const;
    void write(uint16_t addr, std::byte val);

   private:
    std::vector<std::byte> m_data;
};

}  // namespace gb