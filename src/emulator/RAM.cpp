#include "RAM.hpp"

namespace gb {

std::byte WRAM::read(const uint16_t addr) const {
    const uint16_t index = addr - 0xC000;
    return m_data.at(index);
}

void WRAM::write(const uint16_t addr, const std::byte val) {
    const uint16_t index = addr - 0xC000;
    m_data[index] = val;
}

std::byte HRAM::read(const uint16_t addr) const {
    const uint16_t index = addr - 0xFF80;
    return m_data.at(index);
}

void HRAM::write(const uint16_t addr, const std::byte val) {
    const uint16_t index = addr - 0xFF80;
    m_data[index] = val;
}

std::byte VRAM::read(const uint16_t addr) const {
    const uint16_t index = addr - 0x8000;
    return m_data.at(index);
}

void VRAM::write(const uint16_t addr, std::byte val) {
    const uint16_t index = addr - 0x8000;
    m_data[index] = val;
}

std::byte OAM::read(const uint16_t addr) const {
    const uint16_t index = addr - 0xFE00;
    return m_data.at(index);
}

void OAM::write(const uint16_t addr, const std::byte val) {
    const uint16_t index = addr - 0xFE00;
    m_data[index] = val;
}

}  // namespace gb