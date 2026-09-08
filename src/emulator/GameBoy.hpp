#pragma once

#include "Cartridge.hpp"

namespace gb {

class Cartridge;

class GameBoy {
   public:
    explicit GameBoy(Cartridge&& cartridge);

   private:
    Cartridge m_cartridge;
};

}  // namespace gb