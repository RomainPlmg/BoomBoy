#pragma once

#include <cstdint>

#include "InterruptHandler.hpp"
#include "Registers.hpp"

namespace gb {

enum class R8 : uint8_t {
    B = 0,
    C,
    D,
    E,
    H,
    L,
    pHL,
    A,
};

enum class R16 : uint8_t {
    BC = 0,
    DE,
    HL,
    SP,
};

enum class R16Stk : uint8_t {
    BC = 0,
    DE,
    HL,
    AF,
};

enum class R16Mem : uint8_t {
    BC = 0,
    DE,
    HLi,
    HLd,
};

enum class Cond : uint8_t {
    NZ = 0, /**< Execute if Z is not set. */
    Z,      /**< Execute if Z is set. */
    NC,     /**< Execute if C is set. */
    C,      /**< Execute if C is not set. */
};

class Bus;

class CPU {
   public:
    explicit CPU(Bus& bus, InterruptHandler& interruptHandler);

    void reset();
    uint32_t step();

    [[nodiscard]] const Registers& regs() const { return m_regs; }
    [[nodiscard]] Registers& regs() { return m_regs; }
    [[nodiscard]] uint8_t currentOP() const { return m_currentOP; }

   private:
    Bus& m_bus;
    InterruptHandler& m_interruptHandler;
    Registers m_regs;

    uint8_t m_currentOP = 0x00;
    bool m_requestIMEEn = false;
    bool m_halted = false;
    bool m_haltBug = false;

    [[nodiscard]] uint8_t fetch8();
    [[nodiscard]] uint16_t fetch16();
    [[nodiscard]] uint8_t pop8();
    [[nodiscard]] uint16_t pop16();
    void push8(uint8_t val);
    void push16(uint16_t val);

    void execute(uint8_t opcode);
    void executeCB(uint8_t opcode);

    void writeReg8(R8 operand, uint8_t val);
    uint8_t readReg8(R8 operand);

    void writeReg16(R16 operand, uint16_t val);
    void writeReg16(R16Stk operand, uint16_t val);
    void writeReg16(R16Mem operand, uint16_t val);
    uint16_t readReg16(R16 operand);
    uint16_t readReg16(R16Stk operand);
    uint16_t readReg16(R16Mem operand);

    bool checkCond(Cond cond);

    bool handleInterrupts();
};

}  // namespace gb