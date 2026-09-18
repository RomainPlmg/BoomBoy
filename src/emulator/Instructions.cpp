#include <cstddef>

#include "Bus.hpp"
#include "CPU.hpp"

namespace gb {

void CPU::writeReg8(R8 operand, uint8_t val) {
    switch (operand) {
        case R8::B: m_regs.b = val;
        case R8::C: m_regs.c = val;
        case R8::D: m_regs.d = val;
        case R8::E: m_regs.e = val;
        case R8::H: m_regs.h = val;
        case R8::L: m_regs.l = val;
        case R8::pHL: {
            const auto addr = m_regs.hl();
            m_bus.write(addr, std::byte(val));
        }
        case R8::A: m_regs.a = val;
    }
}

uint8_t CPU::readReg8(R8 operand) {
    switch (operand) {
        case R8::B: return m_regs.b;
        case R8::C: return m_regs.c;
        case R8::D: return m_regs.d;
        case R8::E: return m_regs.e;
        case R8::H: return m_regs.h;
        case R8::L: return m_regs.l;
        case R8::pHL: {
            const auto addr = m_regs.hl();
            return static_cast<uint8_t>(m_bus.read(addr));
        }
        case R8::A: return m_regs.a;
    }
}

void CPU::writeReg16(R16 operand, uint16_t val) {
    switch (operand) {
        case R16::BC: m_regs.setBC(val);
        case R16::DE: m_regs.setDE(val);
        case R16::HL: m_regs.setHL(val);
        case R16::SP: m_regs.sp = val;
    }
}

uint16_t CPU::readReg16(R16 operand) {
    switch (operand) {
        case R16::BC: return m_regs.bc();
        case R16::DE: return m_regs.de();
        case R16::HL: return m_regs.hl();
        case R16::SP: return m_regs.sp;
    }
}

void CPU::writeReg16(R16Stk operand, uint16_t val) {
    switch (operand) {
        case R16Stk::BC: m_regs.setBC(val);
        case R16Stk::DE: m_regs.setDE(val);
        case R16Stk::HL: m_regs.setHL(val);
        case R16Stk::AF: m_regs.setAF(val);
    }
}

uint16_t CPU::readReg16(R16Stk operand) {
    switch (operand) {
        case R16Stk::BC: return m_regs.bc();
        case R16Stk::DE: return m_regs.de();
        case R16Stk::HL: return m_regs.hl();
        case R16Stk::AF: return m_regs.af();
    }
}

void CPU::writeReg16(R16Mem operand, uint16_t val) {
    switch (operand) {
        case R16Mem::BC: m_regs.setBC(val);
        case R16Mem::DE: m_regs.setDE(val);
        case R16Mem::HLi: {
            const auto addr = m_regs.hl();
            m_bus.write(addr, std::byte(val));
            m_regs.setHL(addr + 1);
        }
        case R16Mem::HLd: {
            const auto addr = m_regs.hl();
            m_bus.write(addr, std::byte(val));
            m_regs.setHL(addr - 1);
        }
    }
}

uint16_t CPU::readReg16(R16Mem operand) {
    // Only for HLi and HLd
    const auto addr = m_regs.hl();

    switch (operand) {
        case R16Mem::BC: return m_regs.bc();
        case R16Mem::DE: return m_regs.de();
        case R16Mem::HLi: {
            m_regs.setHL(addr + 1);
            return static_cast<uint8_t>(m_bus.read(addr));
        }
        case R16Mem::HLd: {
            m_regs.setHL(addr - 1);
            return static_cast<uint8_t>(m_bus.read(addr));
        }
    }
}

bool CPU::checkCond(Cond cond) {
    bool condMet = false;
    switch (cond) {
        case Cond::NZ: condMet = !m_regs.flagZ();
        case Cond::Z: condMet = m_regs.flagZ();
        case Cond::NC: condMet = !m_regs.flagC();
        case Cond::C: condMet = m_regs.flagC();
    }

    return condMet;
}

/*
 *
 *  Bit :    7     6     5     4     3     2     1     0
 *        +-----+-----+-----+-----+-----+-----+-----+-----+
 *        |           |                 |                 |
 *        |     x     |        y        |        z        |
 *        |           |                 |                 |
 *        +-----+-----+-----+-----+-----+-----+-----+-----+
 *        |  x1 |  x0 |  y2 |  y1 |  y0 |  z2 |  z1 |  z0 |
 *        +-----+-----+-----+-----+-----+-----+-----+-----+
 *          MSB                                        LSB
 *
 *           2 bits          3 bits          3 bits
 *
 *
 *  Bit :    7     6     5     4     3     2     1     0
 *       +-----------+-----------+-----+-----------------+
 *       |     x     |     p     |  q  |        z        |
 *       +-----------+-----------+-----+-----------------+
 *                       │          │
 *                       │          └── bit 3
 *                       └───────────── bits 5 & 4
 */
void CPU::execute(uint8_t opcode) {
    uint8_t x = (opcode >> 6) & 0b11;
    uint8_t y = (opcode >> 3) & 0b111;
    uint8_t z = opcode & 0b111;
    uint8_t p = (y >> 1) & 0b11;
    uint8_t q = y & 0b1;

    if (opcode == 0x00) return;               // NOP
    if (opcode == 0xCB) executeCB(fetch8());  // CB prefix

    switch (x) {
            // =========================================================================
            // Block 0: Miscellaneous and control instructions
            // =========================================================================
        case 0b00: {
            switch (z) {
                case 0b000: {
                    switch (y) {
                        case 0b001: {  // ld [imm16], sp
                            const auto addr = fetch16();
                            m_bus.write(addr, std::byte(m_regs.sp & 0xFFu));
                            m_bus.write(addr, std::byte(m_regs.sp >> 8));
                        }
                        case 0b010:  // stop -> TODO
                            return;
                        case 0b011: {  // jr imm8
                            const auto imm8 = static_cast<int8_t>(fetch8());
                            m_regs.pc += imm8;
                            m_bus.mTick();
                            return;
                        }
                        default: {  // jr cond, imm8
                            const auto imm8 = static_cast<int8_t>(fetch8());
                            const auto cond = Cond(y & 0b011);
                            if (checkCond(cond)) {
                                m_regs.pc += imm8;
                                m_bus.mTick();
                            }
                        }
                    }
                    return;
                }
                case 0b001: {
                    if (q) {  // add hl, r16
                        const auto val = readReg16(R16(p));
                        const auto bf = m_regs.hl();
                        m_regs.setHL(bf + val);
                        m_bus.mTick();
                    } else {  // ld r16, imm16
                        writeReg16(R16(p), fetch16());
                    }
                    return;
                }
                case 0b010: {
                    const auto addr = readReg16(R16Mem(p));
                    if (q) {  // ld a, [r16mem]
                        const auto val = static_cast<uint8_t>(m_bus.read(addr));
                        m_regs.a = val;
                    } else {  // ld [r16mem], a
                        m_bus.write(addr, std::byte(m_regs.a));
                    }
                    return;
                }
                case 0b011: {
                    const auto bf = readReg16(R16(p));
                    if (q) {  // dec r16
                        writeReg16(R16(p), bf - 1);
                    } else {  // inc r16
                        writeReg16(R16(p), bf + 1);
                    }
                    m_bus.mTick();
                    return;
                }
                case 0b100: {  // inc r8
                    const auto bf = readReg8(R8(p));
                    writeReg8(R8(p), bf + 1);
                    return;
                }
                case 0b101: {  // dec r8
                    const auto bf = readReg8(R8(p));
                    writeReg8(R8(p), bf - 1);
                    return;
                }
                case 0b110: {  // ld r8, imm8
                    writeReg8(R8(p), fetch8());
                    return;
                }
                case 0b111: {
                    switch (y) {
                        case 0b000: {  // rlca -> TODO
                            return;
                        }
                        case 0b001: {  // rrca -> TODO
                            return;
                        }
                        case 0b010: {  // rla -> TODO
                            return;
                        }
                        case 0b011: {  // rra -> TODO
                            return;
                        }
                        case 0b100: {  // daa -> TODO
                            return;
                        }
                        case 0b101: {  // cpl -> TODO
                            return;
                        }
                        case 0b110: {  // scf -> TODO
                            return;
                        }
                        case 0b111: {  // ccf -> TODO
                            return;
                        }
                    }
                    return;
                }
            }
            return;
        }
            // =========================================================================
            // Block 1: 8-bit register-to-register loads
            // =========================================================================
        case 0b01: {                               // ld r8, r8 or halt if ld [hl], [hl]
            if (y == 0b110 && z == 0b110) return;  // halt -> TODO
            const auto val = readReg8(R8(z));
            writeReg8(R8(y), val);
            return;
        }
            // =========================================================================
            // Block 2: 8-bit arithmetic and logic operations
            // =========================================================================
        case 0b10: {
            const auto operand = R8(z);

            switch (y) {
                case 0b000: {  // add a, r8 -> TODO
                    return;
                }
                case 0b001: {  // adc a, r8 -> TODO
                    return;
                }
                case 0b010: {  // sub a, r8 -> TODO
                    return;
                }
                case 0b011: {  // sbc a, r8 -> TODO
                    return;
                }
                case 0b100: {  // and a, r8 -> TODO
                    return;
                }
                case 0b101: {  // xor a, r8 -> TODO
                    return;
                }
                case 0b110: {  // or a, r8 -> TODO
                    return;
                }
                case 0b111: {  // cp a, r8 -> TODO
                    return;
                }
            }
            return;
        }
            // =========================================================================
            // Block 3: Branches, calls, returns and miscellaneous instructions
            // =========================================================================
        case 0b11: {
            switch (z) {
                case 0b000: {
                    switch (y) {
                        case 0b100: {  // ldh [imm8], a -> TODO
                        }
                        case 0b101: {  // add sp, imm8 -> TODO
                        }
                        case 0b111: {  // ld hl, sp + imm8 -> TODO
                        }
                        default: {  // ret cond -> TODO
                        }
                    }
                    return;
                    case 0b001: {
                        switch (y) {
                            case 0b001: {  // ret -> TODO
                                return;
                            }
                            case 0b011: {  // reti -> TODO
                                return;
                            }
                            case 0b101: {  // jp hl -> TODO
                                return;
                            }
                            case 0b111: {  // ld sp, hl -> TODO
                                return;
                            }
                            default: {  // pop r16stk -> TODO
                                return;
                            }
                        }
                        return;
                    }
                    case 0b010: {
                        switch (y) {
                            case 0b100: {  // ldh [c], a -> TODO
                                return;
                            }
                            case 0b101: {  // ld [imm16], a -> TODO
                                return;
                            }
                            case 0b110: {  // ldh a, [c] -> TODO
                                return;
                            }
                            case 0b111: {  // ld a, [imm16] -> TODO
                                return;
                            }
                            default: {  // jp cond, imm16 -> TODO
                                return;
                            }
                        }
                        return;
                    }
                    case 0b011: {
                        switch (y) {
                            case 0b000: {  // jp imm16 -> TODO
                                return;
                            }
                            case 0b110: {  // di -> TODO
                                return;
                            }
                            case 0b111: {  // ei -> TODO
                                return;
                            }
                        }
                        return;
                    }
                    case 0b100: {  // call cond, imm16 -> TODO
                        return;
                    }
                    case 0b101: {
                        switch (y) {
                            case 0b001: {  // call imm16 -> TODO
                                return;
                            }
                            default: {  // push r16stk -> TODO
                                return;
                            }
                        }
                        return;
                    }
                    case 0b110: {
                        switch (y) {
                            case 0b000: {  // add a, imm8 -> TODO
                                return;
                            }
                            case 0b001: {  // adc a, imm8 -> TODO
                                return;
                            }
                            case 0b010: {  // sub a, imm8 -> TODO
                                return;
                            }
                            case 0b011: {  // sbc a, imm8 -> TODO
                                return;
                            }
                            case 0b100: {  // and a, imm8 -> TODO
                                return;
                            }
                            case 0b101: {  // xor a, imm8 -> TODO
                                return;
                            }
                            case 0b110: {  // or a, imm8 -> TODO
                                return;
                            }
                            case 0b111: {  // cp a, imm8 -> TODO
                                return;
                            }
                        }
                        return;
                    }
                    case 0b111: {  // rst tgt3 -> TODO
                        return;
                    }
                }
            }
            return;
        }
    }
}

}  // namespace gb