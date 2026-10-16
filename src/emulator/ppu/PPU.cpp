#include "PPU.hpp"

#include "InterruptHandler.hpp"
#include "Log.hpp"
#include "TileMap.hpp"

namespace gb {

void PPU::write(uint16_t addr, std::byte val) {
    const auto data = static_cast<uint8_t>(val);

    if (addr == 0XFF40) {
        if ((data & LCD_LCDC_LCDEn_Msk) && (m_currentMode != Mode::VBLANK)) {
            // LOG_ERROR(
            //     "You are trying to stop LCD operation outside the VBLANK period!\nDisabling the display outside of
            //     the " "VBLANK period may damage the hardware on real GameBoy.");
        }
        m_regs.LCDC = data;
    } else if (addr == 0xFF41) {
        m_regs.STAT = data;
    } else if (addr == 0xFF42) {
        m_regs.SCY = data;
    } else if (addr == 0xFF43) {
        m_regs.SCX = data;
    } else if (addr == 0xFF44) {
        LOG_WARN("LY is read-only!");
    } else if (addr == 0xFF45) {
        m_regs.LYC = data;
    }
}

[[nodiscard]] std::byte PPU::read(uint16_t addr) const {
    std::byte val{0xFF};

    if (addr == 0XFF40) {
        val = std::byte(m_regs.LCDC);
    } else if (addr == 0xFF41) {
        val = std::byte(m_regs.STAT);
    } else if (addr == 0xFF42) {
        val = std::byte(m_regs.SCY);
    } else if (addr == 0xFF43) {
        val = std::byte(m_regs.SCX);
    } else if (addr == 0xFF44) {
        val = std::byte(m_regs.LY);
    } else if (addr == 0xFF45) {
        val = std::byte(m_regs.LYC);
    }

    return val;
}

void PPU::tick() {
    // Check if the LCD is enabled
    if (!(m_regs.LCDC & LCD_LCDC_LCDEn_Msk)) {
        m_dot = 0;
        m_regs.LY = 0;
        m_currentMode = Mode::HBLANK;
        return;
    }

    // PPU state machine
    switch (m_currentMode) {
        case Mode::HBLANK: {
            if (m_dot > 455) {  // The end of the scanline
                incrementLY();
                m_dot = 0;
                if (m_regs.LY == 144) {  // All 144 scanlines have been drawn, switch to the 10 VBLANK scanlines
                    m_currentMode = Mode::VBLANK;
                    m_interruptHandler.toggleInterrupt(InterruptSource::VBLANK);
                } else {
                    m_currentMode = Mode::OAMSCAN;
                }
                return;
            }
            m_dot++;
            break;
        }
        case Mode::VBLANK: {
            if (m_dot > 455) {
                incrementLY();
                m_dot = 0;
                if (m_regs.LY == 153) {  // The last scanline
                    m_regs.LY = 0;       // Start a new frame
                    m_currentMode = Mode::OAMSCAN;
                }
                return;
            }
            m_dot++;
            break;
        }
        case Mode::OAMSCAN: {
            if (m_dot == 79) {  // OAM scan has a fixed number of dot (80)
                // TODO -> Evaluate sprites
                m_currentMode = Mode::DRAW;
            }
            m_dot++;
            break;
        }
        case Mode::DRAW: {
            if (m_dot == 252) {
                renderScanline();
                m_currentMode = Mode::HBLANK;
            }
            m_dot++;
            break;
        }
    }
}

void PPU::incrementLY() {
    m_regs.LY = (m_regs.LY + 1) % 154;  // 144 lines + 10 VBLANK
    if (m_regs.LY == m_regs.LYC) {      // Check if LY is equal the the LY compare register content
        // If it is the case, set it in the LCD Status register and generate the appropriate interrupt,
        // if this one is enabled
        m_regs.STAT |= LCD_STAT_LYCeLY(true);
        if (m_regs.STAT & LCD_STAT_LYCInt_Pos) {
            m_interruptHandler.toggleInterrupt(InterruptSource::LCD);  // Toggle LCD interrupt
        }
    } else {
        // Else clear the LY = LYC bit in STAT register
        m_regs.STAT &= ~LCD_STAT_LYCeLY_Msk;
    }
}

void PPU::renderScanline() {
    // Check if Background & Window are enabled
    if (m_regs.LCDC & LCD_LCDC_BGWEn_Msk) {
        renderBGScanline();
        if (m_regs.LCDC & LCD_LCDC_WindowEn_Msk) {
            // TODO -> Render Window scanline
        }
    }

    if (m_regs.LCDC & LCD_LCDC_OBJEn_Msk) {
        // TODO -> Render Objects on scanline
    }
}

void PPU::renderBGScanline() {
    // Calculate the top left & corner of the background area
    Point topLeft = {m_regs.SCX, m_regs.SCY};

    // Calculate the line number of the background to render
    Point pixelCoord{};
    pixelCoord.y = (topLeft.y + m_regs.LY) % 256;

    for (uint16_t x = 0; x < 160; x++) {
        pixelCoord.x = x + topLeft.x;
        auto color = TileMap::getBackgroundPixelColor(m_vram, m_regs, pixelCoord);
        m_fb.setPixel(x, m_regs.LY, static_cast<uint8_t>(color));
    }
}

}  // namespace gb