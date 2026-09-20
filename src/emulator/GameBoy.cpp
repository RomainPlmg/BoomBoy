#include "GameBoy.hpp"

#include "Bus.hpp"
#include "Cartridge.hpp"

namespace gb {

GameBoy::GameBoy(Cartridge&& cartridge)
    : m_cartridge(std::move(cartridge)),
      m_bus(m_cartridge, m_wram, m_hram, m_vram, m_oam, m_interruptHandler, m_serialLink),
      m_interruptHandler(m_bus),
      m_cpu(m_bus, m_interruptHandler) {
    m_cpu.reset();
}

uint32_t GameBoy::step() { return m_cpu.step(); }

}  // namespace gb