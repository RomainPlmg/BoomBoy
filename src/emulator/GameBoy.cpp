#include "GameBoy.hpp"

#include "Cartridge.hpp"

namespace gb {

GameBoy::GameBoy(Cartridge&& cartridge) : m_cartridge(std::move(cartridge)) {}

}  // namespace gb