#include "CPU.hpp"

namespace gb {

void CPU::executeCB(uint8_t opcode) {
    uint8_t x = (opcode >> 6) & 0b11;
    uint8_t y = (opcode >> 3) & 0b111;
    uint8_t z = opcode & 0b111;
    uint8_t p = (y >> 1) & 0b11;
    uint8_t q = y & 0b1;
}

}  // namespace gb