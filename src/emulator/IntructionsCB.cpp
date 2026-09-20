#include "CPU.hpp"

namespace gb {

void CPU::executeCB(uint8_t opcode) {
    uint8_t x = (opcode >> 6) & 0b11;
    uint8_t y = (opcode >> 3) & 0b111;
    uint8_t z = opcode & 0b111;
    uint8_t p = (y >> 1) & 0b11;
    uint8_t q = y & 0b1;

    const auto r8 = readReg8(R8(z));

    switch (x) {
        case 0b00: {
            switch (y) {
                case 0b000: {  // rlc r8
                    bool carry = r8 & 0x80;
                    const auto res = (r8 << 1) | (carry ? 1 : 0);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = carry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
                case 0b001: {  // rrc r8
                    bool carry = r8 & 0x01;
                    const auto res = (r8 >> 1) | (carry << 7);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = carry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
                case 0b010: {  // rl r8
                    bool carry = r8 & 0x80;
                    bool oldCarry = m_regs.flagC();
                    const uint8_t res = (r8 << 1) | (oldCarry ? 1 : 0);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = carry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
                case 0b011: {  // rr r8
                    bool newCarry = r8 & 0x01;
                    bool carry = m_regs.flagC();
                    const auto res = (r8 >> 1) | (carry << 7);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = newCarry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
                case 0b100: {  // sla r8
                    uint8_t carry = r8 & 0x80;
                    const uint8_t res = (r8 << 1);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = carry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
                case 0b101: {  // sra r8
                    bool carry = r8 & 0x01;
                    bool signBit = r8 & 0x80;
                    const auto res = (r8 >> 1) | (signBit << 7);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = carry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
                case 0b110: {  // swap r8
                    const uint8_t low = r8 & 0x0F;
                    const uint8_t high = r8 & 0xF0;

                    const auto res = (low << 4) | (high >> 4);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, FlagOp::Clear);
                    return;
                }
                case 0b111: {  // srl r8
                    bool carry = r8 & 0x01;
                    const auto res = (r8 >> 1);
                    writeReg8(R8(z), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = carry ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, flagOpC);
                    return;
                }
            }
            return;
        }
        case 0b01: {  // bit b3, r8
            const auto res = r8 & (1 << y);

            FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
            m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Set, FlagOp::Keep);
            return;
        }
        case 0b10: {  // res b3, r8
            const auto res = r8 & ~(1 << y);
            writeReg8(R8(z), res);
            return;
        }
        case 0b11: {  // set b3, r8
            const auto res = r8 | (1 << y);
            writeReg8(R8(z), res);
            return;
        }
    }
}

}  // namespace gb