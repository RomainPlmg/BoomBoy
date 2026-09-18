#include "GameBoy.hpp"

#include "Bus.hpp"
#include "Cartridge.hpp"

namespace gb {

GameBoy::GameBoy(Cartridge&& cartridge) : m_cartridge(std::move(cartridge)), m_bus(m_cartridge), m_cpu(m_bus) {
    m_cpu.reset();
}

uint32_t GameBoy::step() {
    return m_cpu.step();
}

}  // namespace gb