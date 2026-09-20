#include <cstddef>

#include "Bus.hpp"
#include "CPU.hpp"

namespace gb {

void CPU::writeReg8(R8 operand, uint8_t val) {
    switch (operand) {
        case R8::B: {
            m_regs.b = val;
            return;
        }
        case R8::C: {
            m_regs.c = val;
            return;
        }
        case R8::D: {
            m_regs.d = val;
            return;
        }
        case R8::E: {
            m_regs.e = val;
            return;
        }
        case R8::H: {
            m_regs.h = val;
            return;
        }
        case R8::L: {
            m_regs.l = val;
            return;
        }
        case R8::pHL: {
            const auto addr = m_regs.hl();
            m_bus.write(addr, std::byte(val));
            return;
        }
        case R8::A: {
            m_regs.a = val;
            return;
        }
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
        case R16::BC: {
            m_regs.setBC(val);
            return;
        }
        case R16::DE: {
            m_regs.setDE(val);
            return;
        }
        case R16::HL: {
            m_regs.setHL(val);
            return;
        }
        case R16::SP: {
            m_regs.sp = val;
            return;
        }
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
        case R16Stk::BC: {
            m_regs.setBC(val);
            return;
        }
        case R16Stk::DE: {
            m_regs.setDE(val);
            return;
        }
        case R16Stk::HL: {
            m_regs.setHL(val);
            return;
        }
        case R16Stk::AF: {
            m_regs.setAF(val & 0xFFF0);
            return;
        }
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

uint16_t CPU::readReg16(R16Mem operand) {
    // Only for HLi and HLd
    const auto addr = m_regs.hl();

    switch (operand) {
        case R16Mem::BC: return m_regs.bc();
        case R16Mem::DE: return m_regs.de();
        case R16Mem::HLi: {
            m_regs.setHL(addr + 1);
            return addr;
        }
        case R16Mem::HLd: {
            m_regs.setHL(addr - 1);
            return addr;
        }
    }
}

bool CPU::checkCond(Cond cond) {
    bool condMet = false;
    switch (cond) {
        case Cond::NZ: {
            condMet = !m_regs.flagZ();
            break;
        }
        case Cond::Z: {
            condMet = m_regs.flagZ();
            break;
        }
        case Cond::NC: {
            condMet = !m_regs.flagC();
            break;
        }
        case Cond::C: {
            condMet = m_regs.flagC();
            break;
        }
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

    if (opcode == 0x00) return;  // NOP
    if (opcode == 0xCB) {        // CB prefix
        executeCB(fetch8());
        return;
    }

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
                            m_bus.write(addr + 1, std::byte(m_regs.sp >> 8));
                            return;
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
                            const auto cond = y & 0b011;
                            if (checkCond(Cond(cond))) {
                                m_regs.pc += imm8;
                                m_bus.mTick();
                            }
                            return;
                        }
                    }
                    return;
                }
                case 0b001: {
                    if (q) {  // add hl, r16
                        const auto val = readReg16(R16(p));
                        const auto bf = m_regs.hl();
                        const uint32_t res = bf + val;
                        m_regs.setHL(res & 0x0000FFFF);

                        FlagOp flagOpH = ((bf & 0x0FFF) + (val & 0x0FFF)) > 0x0FFF ? FlagOp::Set : FlagOp::Clear;
                        FlagOp flagOpC = res > 0xFFFF ? FlagOp::Set : FlagOp::Clear;
                        m_regs.setFlags(FlagOp::Keep, FlagOp::Clear, flagOpH, flagOpC);

                        m_bus.mTick();
                    } else {  // ld r16, imm16
                        writeReg16(R16(p), fetch16());
                    }
                    return;
                }
                case 0b010: {
                    const auto addr = readReg16(R16Mem(p));
                    if (q) {  // ld a, [r16mem]
                        m_regs.a = static_cast<uint8_t>(m_bus.read(addr));
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
                    const auto bf = readReg8(R8(y));
                    const uint8_t res = bf + 1;
                    writeReg8(R8(y), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = (bf & 0x0F) == 0x0F ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, flagOpH, FlagOp::Keep);
                    return;
                }
                case 0b101: {  // dec r8
                    const auto bf = readReg8(R8(y));
                    const uint8_t res = bf - 1;
                    writeReg8(R8(y), res);

                    FlagOp flagOpZ = res == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = (bf & 0x0F) == 0x00 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, FlagOp::Keep);
                    return;
                }
                case 0b110: {  // ld r8, imm8
                    writeReg8(R8(y), fetch8());
                    return;
                }
                case 0b111: {
                    switch (y) {
                        case 0b000: {  // rlca
                            const auto carry = (m_regs.a & 0x80) >> 7;
                            m_regs.a = (m_regs.a << 1) | carry;

                            m_regs.setFlags(FlagOp::Clear, FlagOp::Clear, FlagOp::Clear,
                                            carry ? FlagOp::Set : FlagOp::Clear);
                            return;
                        }
                        case 0b001: {  // rrca
                            const auto carry = m_regs.a & 0x01;
                            m_regs.a = (m_regs.a >> 1) | (carry << 7);

                            m_regs.setFlags(FlagOp::Clear, FlagOp::Clear, FlagOp::Clear,
                                            carry ? FlagOp::Set : FlagOp::Clear);
                            return;
                        }
                        case 0b010: {  // rla
                            const auto oldCarry = m_regs.flagC() ? 1 : 0;
                            const auto carry = (m_regs.a & 0x80) >> 7;
                            m_regs.a = (m_regs.a << 1) | oldCarry;

                            m_regs.setFlags(FlagOp::Clear, FlagOp::Clear, FlagOp::Clear,
                                            carry ? FlagOp::Set : FlagOp::Clear);
                            return;
                        }
                        case 0b011: {  // rra
                            const auto oldCarry = m_regs.flagC() ? 1 : 0;
                            const auto carry = m_regs.a & 0x01;
                            m_regs.a = (m_regs.a >> 1) | (oldCarry << 7);

                            m_regs.setFlags(FlagOp::Clear, FlagOp::Clear, FlagOp::Clear,
                                            carry ? FlagOp::Set : FlagOp::Clear);
                            return;
                        }
                        case 0b100: {  // daa
                            uint8_t adjustment = 0;
                            bool setC = false;
                            if (m_regs.flagN()) {
                                if (m_regs.flagH()) adjustment += 0x06;
                                if (m_regs.flagC()) {
                                    adjustment += 0x60;
                                    setC = true;
                                }
                                m_regs.a -= adjustment;
                            } else {
                                if (m_regs.flagH() || (m_regs.a & 0x0F) > 0x09) adjustment += 0x06;
                                if (m_regs.flagC() || m_regs.a > 0x99) {
                                    adjustment += 0x60;
                                    setC = true;
                                }
                                m_regs.a += adjustment;
                            }

                            m_regs.setFlags(m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear, FlagOp::Keep, FlagOp::Clear,
                                            setC ? FlagOp::Set : FlagOp::Clear);
                            return;
                        }
                        case 0b101: {  // cpl
                            m_regs.a = ~m_regs.a;

                            m_regs.setFlags(FlagOp::Keep, FlagOp::Set, FlagOp::Set, FlagOp::Keep);
                            return;
                        }
                        case 0b110: {  // scf
                            m_regs.setFlags(FlagOp::Keep, FlagOp::Clear, FlagOp::Clear, FlagOp::Set);
                            return;
                        }
                        case 0b111: {  // ccf
                            bool c = m_regs.flagC() ? false : true;
                            m_regs.setFlags(FlagOp::Keep, FlagOp::Clear, FlagOp::Clear,
                                            c ? FlagOp::Set : FlagOp::Clear);
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
        case 0b01: {                         // ld r8, r8 or halt if ld [hl], [hl]
            if (y == 0b110 && z == 0b110) {  // halt
                m_halted = true;
                return;
            }

            const auto val = readReg8(R8(z));
            writeReg8(R8(y), val);
            return;
        }
            // =========================================================================
            // Block 2: 8-bit arithmetic and logic operations
            // =========================================================================
        case 0b10: {
            const auto r8 = readReg8(R8(z));
            const auto backupA = m_regs.a;

            switch (y) {
                case 0b000: {  // add a, r8
                    const uint16_t res = m_regs.a + r8;
                    m_regs.a = res & 0xFF;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = ((backupA & 0x0F) + (r8 & 0x0F) > 0x0F) ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = res > 0xFF ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, flagOpH, flagOpC);
                    return;
                }
                case 0b001: {  // adc a, r8
                    const uint8_t carry = m_regs.flagC() ? 1 : 0;
                    const uint16_t res = m_regs.a + r8 + carry;
                    m_regs.a = res & 0xFF;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = ((backupA & 0x0F) + (r8 & 0x0F) + carry > 0x0F) ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = res > 0xFF ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, flagOpH, flagOpC);
                    return;
                }
                case 0b010: {  // sub a, r8
                    const uint16_t res = m_regs.a - r8;
                    m_regs.a = res & 0xFF;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = ((backupA & 0x0F) < (r8 & 0x0F)) ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = backupA < r8 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, flagOpC);
                    return;
                }
                case 0b011: {  // sbc a, r8
                    const uint8_t carry = m_regs.flagC() ? 1 : 0;
                    const uint16_t res = m_regs.a - r8 - carry;
                    m_regs.a = res & 0xFF;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = (((backupA ^ r8 ^ res) & 0x10) != 0) ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = (backupA < r8 + carry) ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, flagOpC);
                    return;
                }
                case 0b100: {  // and a, r8
                    m_regs.a &= r8;
                    uint8_t flags = Flags::HALFC;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Set, FlagOp::Clear);
                    return;
                }
                case 0b101: {  // xor a, r8
                    m_regs.a ^= r8;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, FlagOp::Clear);
                    return;
                }
                case 0b110: {  // or a, r8
                    m_regs.a |= r8;

                    FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, FlagOp::Clear);
                    return;
                }
                case 0b111: {  // cp a, r8
                    const uint16_t res = m_regs.a - r8;

                    FlagOp flagOpZ = (res & 0xFF) == 0 ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpH = (m_regs.a & 0x0F) < (r8 & 0x0F) ? FlagOp::Set : FlagOp::Clear;
                    FlagOp flagOpC = m_regs.a < r8 ? FlagOp::Set : FlagOp::Clear;
                    m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, flagOpC);
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
                        case 0b100: {  // ldh [imm8], a
                            const auto addr = 0xFF00u + fetch8();
                            m_bus.write(addr, std::byte(m_regs.a));
                            return;
                        }
                        case 0b101: {  // add sp, imm8
                            const auto val = static_cast<int8_t>(fetch8());
                            uint32_t res = m_regs.sp + val;
                            const auto backupSP = m_regs.sp;
                            m_regs.sp = res & 0xFFFFu;

                            FlagOp flagOpH = (((backupSP & 0x0F) + (val & 0x0F)) > 0x0F) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = (((backupSP & 0xFF) + (val & 0xFF)) > 0xFF) ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(FlagOp::Clear, FlagOp::Clear, flagOpH, flagOpC);

                            m_bus.mTick(2);
                            return;
                        }
                        case 0b110: {  // ldh a, [imm8]
                            const auto addr = 0xFF00u + fetch8();
                            m_regs.a = static_cast<uint8_t>(m_bus.read(addr));
                            return;
                        }
                        case 0b111: {  // ld hl, sp + imm8
                            const auto val = static_cast<int8_t>(fetch8());
                            uint32_t res = m_regs.sp + val;
                            m_regs.setHL(res & 0xFFFFu);

                            FlagOp flagOpH = (((m_regs.sp & 0x0F) + (val & 0x0F)) > 0x0F) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = (((m_regs.sp & 0xFF) + (val & 0xFF)) > 0xFF) ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(FlagOp::Clear, FlagOp::Clear, flagOpH, flagOpC);

                            m_bus.mTick();
                            return;
                        }
                        default: {  // ret cond
                            const auto cond = y & 0b011;
                            if (checkCond(Cond(cond))) {
                                m_regs.pc = pop16();
                                m_bus.mTick();
                            }
                            m_bus.mTick();
                        }
                    }
                    return;
                }
                case 0b001: {
                    switch (y) {
                        case 0b001: {  // ret
                            m_regs.pc = pop16();
                            m_bus.mTick();
                            return;
                        }
                        case 0b011: {  // reti
                            m_regs.pc = pop16();
                            m_interruptHandler.ime = true;
                            m_bus.mTick();
                            return;
                        }
                        case 0b101: {  // jp hl
                            m_regs.pc = m_regs.hl();
                            return;
                        }
                        case 0b111: {  // ld sp, hl
                            m_regs.sp = m_regs.hl();
                            m_bus.mTick();
                            return;
                        }
                        default: {  // pop r16stk
                            writeReg16(R16Stk(p), pop16());
                            return;
                        }
                    }
                    return;
                }
                case 0b010: {
                    switch (y) {
                        case 0b100: {  // ldh [c], a
                            const auto addr = 0xFF00u + m_regs.c;
                            m_bus.write(addr, std::byte(m_regs.a));
                            return;
                        }
                        case 0b101: {  // ld [imm16], a
                            m_bus.write(fetch16(), std::byte(m_regs.a));
                            return;
                        }
                        case 0b110: {  // ldh a, [c]
                            const auto addr = 0xFF00u + m_regs.c;
                            m_regs.a = static_cast<uint8_t>(m_bus.read(addr));
                            return;
                        }
                        case 0b111: {  // ld a, [imm16]
                            m_regs.a = static_cast<uint8_t>(m_bus.read(fetch16()));
                            return;
                        }
                        default: {  // jp cond, imm16
                            const auto addr = fetch16();
                            const auto cond = y & 0b011;
                            if (checkCond(Cond(cond))) {
                                m_regs.pc = addr;
                                m_bus.mTick();
                            }
                            return;
                        }
                    }
                    return;
                }
                case 0b011: {
                    switch (y) {
                        case 0b000: {  // jp imm16
                            m_regs.pc = fetch16();
                            m_bus.mTick();
                            return;
                        }
                        case 0b110: {  // di
                            m_interruptHandler.ime = false;
                            return;
                        }
                        case 0b111: {  // ei
                            m_requestIMEEn = true;
                            return;
                        }
                    }
                    return;
                }
                case 0b100: {  // call cond, imm16
                    const auto cond = y & 0b011;
                    const auto addr = fetch16();
                    if (checkCond(Cond(cond))) {
                        push16(m_regs.pc);
                        m_regs.pc = addr;
                        m_bus.mTick();
                    }
                    return;
                }
                case 0b101: {
                    switch (y) {
                        case 0b001: {  // call imm16
                            const auto addr = fetch16();
                            push16(m_regs.pc);
                            m_regs.pc = addr;
                            m_bus.mTick();
                            return;
                        }
                        default: {  // push r16stk
                            const auto addr = readReg16(R16Stk(p));
                            push16(addr);
                            m_bus.mTick();
                            return;
                        }
                    }
                    return;
                }
                case 0b110: {
                    const auto imm8 = fetch8();
                    const auto backupA = m_regs.a;

                    switch (y) {
                        case 0b000: {  // add a, imm8
                            const uint16_t res = m_regs.a + imm8;
                            m_regs.a = res & 0xFF;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpH = (((backupA & 0x0F) + (imm8 & 0x0F)) > 0x0F) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = res > 0xFF ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Clear, flagOpH, flagOpC);
                            return;
                        }
                        case 0b001: {  // adc a, imm8
                            const uint8_t carry = m_regs.flagC() ? 1 : 0;
                            const uint16_t res = m_regs.a + imm8 + carry;
                            m_regs.a = res & 0xFF;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpH =
                                (((backupA & 0x0F) + (imm8 & 0x0F) + carry) > 0x0F) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = res > 0xFF ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Clear, flagOpH, flagOpC);
                            return;
                        }
                        case 0b010: {  // sub a, imm8
                            const uint16_t res = m_regs.a - imm8;
                            m_regs.a = res & 0xFF;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpH = (((backupA & 0x0F) < (imm8 & 0x0F))) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = res > 0xFF ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, flagOpC);
                            return;
                        }
                        case 0b011: {  // sbc a, imm8
                            const uint8_t carry = m_regs.flagC() ? 1 : 0;
                            const uint16_t res = m_regs.a - imm8 - carry;
                            m_regs.a = res & 0xFF;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpH = (((backupA ^ imm8 ^ res) & 0x10)) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = res > 0xFF ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, flagOpC);
                            return;
                        }
                        case 0b100: {  // and a, imm8
                            m_regs.a &= imm8;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Set, FlagOp::Clear);
                            return;
                        }
                        case 0b101: {  // xor a, imm8
                            m_regs.a ^= imm8;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, FlagOp::Clear);
                            return;
                        }
                        case 0b110: {  // or a, imm8
                            m_regs.a |= imm8;

                            FlagOp flagOpZ = m_regs.a == 0 ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Clear, FlagOp::Clear, FlagOp::Clear);
                            return;
                        }
                        case 0b111: {  // cp a, imm8
                            const uint16_t res = m_regs.a - imm8;

                            FlagOp flagOpZ = (res & 0xFF) == 0 ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpH = (((m_regs.a & 0x0F) < (imm8 & 0x0F))) ? FlagOp::Set : FlagOp::Clear;
                            FlagOp flagOpC = imm8 > m_regs.a ? FlagOp::Set : FlagOp::Clear;
                            m_regs.setFlags(flagOpZ, FlagOp::Set, flagOpH, flagOpC);
                            return;
                        }
                    }
                    return;
                }
                case 0b111: {  // rst tgt3
                    const uint16_t addr = y * 8;
                    push16(m_regs.pc);
                    m_regs.pc = addr;
                    m_bus.mTick();
                    return;
                }
            }
        }
            return;
    }
}

}  // namespace gb