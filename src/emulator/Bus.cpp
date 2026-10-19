#include "Bus.hpp"

#include "BootRom.hpp"
#include "CPU.hpp"
#include "Cartridge.hpp"
#include "RAM.hpp"
#include "SerialLink.hpp"
#include "Timer.hpp"
#include "ppu/PPU.hpp"

namespace gb {

Bus::Bus(const BootRom& bootRom, Cartridge& cartridge, WRAM& wram, HRAM& hram, VRAM& vram, OAM& oam,
         InterruptHandler& interruptHandler, Timer& timer, PPU& ppu, SerialLink& serialLink)
    : m_bootRom(bootRom),
      m_cartridge(cartridge),
      m_wram(wram),
      m_hram(hram),
      m_vram(vram),
      m_oam(oam),
      m_interruptHandler(interruptHandler),
      m_timer(timer),
      m_ppu(ppu),
      m_serialLink(serialLink) {}

std::byte Bus::read(uint16_t addr) {
    std::byte value{0xFF};

    if (addr <= 0x7FFFu || (addr >= 0xA000u && addr <= 0xBFFFu)) {
        if (addr < 0x0100 && m_bootRomEn) {  // Read Boot Rom
            value = m_bootRom.data()[addr];
        } else {
            value = m_cartridge.read(addr);
        }
    } else if (addr >= 0X8000u && addr <= 0x9FFFu) {
        value = m_vram.read(addr);
    } else if (addr >= 0xC000u && addr <= 0xDFFFu) {
        value = m_wram.read(addr);
    } else if (addr >= 0xFE00u && addr <= 0xFE9Fu) {
        value = m_oam.read(addr);
    } else if (addr >= 0XFF01u && addr <= 0xFF02u) {
        value = m_serialLink.read(addr);
    } else if (addr >= 0xFF04u && addr <= 0xFF07u) {
        value = m_timer.read(addr);
    } else if (addr == 0xFF0Fu || addr == 0xFFFFu) {
        value = m_interruptHandler.read(addr);
    } else if (addr >= 0xFF40u && addr <= 0xFF4Bu) {
        value = m_ppu.read(addr);
    } else if (addr >= 0xFF80u && addr <= 0xFFFEu) {
        value = m_hram.read(addr);
    }

    mTick();
    return value;
}

void Bus::write(uint16_t addr, std::byte val) {
    if (addr <= 0x7FFFu) {
        m_cartridge.write(addr, val);
    } else if (addr >= 0X8000u && addr <= 0x9FFFu) {
        m_vram.write(addr, val);
    } else if (addr >= 0xC000u && addr <= 0xDFFFu) {
        m_wram.write(addr, val);
    } else if (addr >= 0xFE00u && addr <= 0xFE9Fu) {
        m_oam.write(addr, val);
    } else if (addr >= 0XFF01 && addr <= 0xFF02) {
        m_serialLink.write(addr, val);
    } else if (addr >= 0xFF04u && addr <= 0xFF07u) {
        m_timer.write(addr, val);
    } else if (addr == 0xFF0Fu || addr == 0xFFFFu) {
        m_interruptHandler.write(addr, val);
    } else if (addr >= 0xFF40u && addr <= 0xFF4Bu) {
        m_ppu.write(addr, val);
    } else if (addr == 0xFF50 && m_bootRomEn && val != std::byte{0}) {  // BootRom mapping control
        m_bootRomEn = false;
    } else if (addr >= 0xFF80u && addr <= 0xFFFEu) {
        m_hram.write(addr, val);
    }

    mTick();
}

void Bus::tick(uint32_t nb) {
    for (size_t i = 0; i < nb; i++) {
        m_ticks++;
        m_serialLink.tick();
        m_timer.tick();
        m_ppu.tick();
        // APU.tick();
        // etc.
    }
}

void Bus::mTick(uint32_t nb) {
    for (size_t i = 0; i < nb; i++) { tick(4); }
}

}  // namespace gb