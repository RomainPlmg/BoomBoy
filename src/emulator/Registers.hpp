#pragma once

#include <cstdbool>
#include <cstdint>

namespace gb {

struct Registers {
    enum Flags {
        ZERO = 0b10000000,
        SUB = 0b01000000,
        HALFC = 0b00100000,
        CARRY = 0b00010000,
    };

    uint8_t a, f, b, c, d, e, h, l;
    uint16_t sp, pc;

    [[nodiscard]] uint16_t af() const { return (a << 8) | (f & 0xF0); }
    [[nodiscard]] uint16_t bc() const { return (b << 8) | c; }
    [[nodiscard]] uint16_t de() const { return (d << 8) | e; }
    [[nodiscard]] uint16_t hl() const { return (h << 8) | l; }

    [[nodiscard]] bool flagZ() const { return f & Flags::ZERO; }
    [[nodiscard]] bool flagN() const { return f & Flags::SUB; }
    [[nodiscard]] bool flagH() const { return f & Flags::HALFC; }
    [[nodiscard]] bool flagC() const { return f & Flags::CARRY; }

    void setAF(uint16_t af) {
        a = af >> 8;
        f = af & 0x00F0;
    }

    void setBC(uint16_t bc) {
        b = bc >> 8;
        c = bc & 0x00FF;
    }

    void setDE(uint16_t de) {
        d = de >> 8;
        e = de & 0x00FF;
    }

    void setHL(uint16_t hl) {
        h = hl >> 8;
        l = hl & 0x00FF;
    }

    void setFlags(Flags flags) { f = flags & 0xF0; }
};

}  // namespace gb