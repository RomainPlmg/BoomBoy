#pragma once

#include <cstdint>

namespace gb {

/************************************/
/* SERIAL LINK                      */
/************************************/
struct SerialLinkRegs {
    uint8_t SB; /**< Serial transfer data */
    uint8_t SC; /**< Serial transfer control */
};

constexpr uint8_t SerialLink_SC_ClockSel_Pos = 0;
constexpr uint8_t SerialLink_SC_ClockSel_Msk = 1 << SerialLink_SC_ClockSel_Pos;
constexpr uint8_t SerialLink_SC_ClockSel(bool value) {
    return SerialLink_SC_ClockSel_Msk & (value << SerialLink_SC_ClockSel_Pos);
}

constexpr uint8_t SerialLink_SC_ClockSpeed_Pos = 1;
constexpr uint8_t SerialLink_SC_ClockSpeed_Msk = 1 << SerialLink_SC_ClockSpeed_Pos;
constexpr uint8_t SerialLink_SC_ClockSpeed(bool value) {
    return SerialLink_SC_ClockSpeed_Msk & (value << SerialLink_SC_ClockSpeed_Pos);
}

constexpr uint8_t SerialLink_SC_TransferEn_Pos = 7;
constexpr uint8_t SerialLink_SC_TransferEn_Msk = 1 << SerialLink_SC_TransferEn_Pos;
constexpr uint8_t SerialLink_SC_TransferEn(bool value) {
    return SerialLink_SC_TransferEn_Msk & (value << SerialLink_SC_TransferEn_Pos);
}

/************************************/
/* LCD                              */
/************************************/
struct LCDRegs {
    uint8_t LCDC; /**< LCD control */
    uint8_t STAT; /**< LCD status */
    uint8_t SCY;  /**< Background viewport Y position */
    uint8_t SCX;  /**< Background viewport X position */
    uint8_t LY;   /**< LCD Y coordinate [read-only] */
    uint8_t LYC;  /**< LY compare */
};

constexpr uint8_t LCD_LCDC_BGWEn_Pos = 0;
constexpr uint8_t LCD_LCDC_BGWEn_Msk = 1 << LCD_LCDC_BGWEn_Pos;
constexpr uint8_t LCD_LCDC_BGWEn(bool value) { return LCD_LCDC_BGWEn_Msk & (value << LCD_LCDC_BGWEn_Pos); }

constexpr uint8_t LCD_LCDC_OBJEn_Pos = 1;
constexpr uint8_t LCD_LCDC_OBJEn_Msk = 1 << LCD_LCDC_OBJEn_Pos;
constexpr uint8_t LCD_LCDC_OBJEn(bool value) { return LCD_LCDC_OBJEn_Msk & (value << LCD_LCDC_OBJEn_Pos); }

constexpr uint8_t LCD_LCDC_OBJSize_Pos = 2;
constexpr uint8_t LCD_LCDC_OBJSize_Msk = 1 << LCD_LCDC_OBJSize_Pos;
constexpr uint8_t LCD_LCDC_OBJSize(bool value) { return LCD_LCDC_OBJSize_Msk & (value << LCD_LCDC_OBJSize_Pos); }

constexpr uint8_t LCD_LCDC_BGTileMap_Pos = 3;
constexpr uint8_t LCD_LCDC_BGTileMap_Msk = 1 << LCD_LCDC_BGTileMap_Pos;
constexpr uint8_t LCD_LCDC_BGTileMap(bool value) { return LCD_LCDC_BGTileMap_Msk & (value << LCD_LCDC_BGTileMap_Pos); }

constexpr uint8_t LCD_LCDC_BGWTiles_Pos = 4;
constexpr uint8_t LCD_LCDC_BGWTiles_Msk = 1 << LCD_LCDC_BGWTiles_Pos;
constexpr uint8_t LCD_LCDC_BGWTiles(bool value) { return LCD_LCDC_BGWTiles_Msk & (value << LCD_LCDC_BGWTiles_Pos); }

constexpr uint8_t LCD_LCDC_WindowEn_Pos = 5;
constexpr uint8_t LCD_LCDC_WindowEn_Msk = 1 << LCD_LCDC_WindowEn_Pos;
constexpr uint8_t LCD_LCDC_WindowEn(bool value) { return LCD_LCDC_WindowEn_Msk & (value << LCD_LCDC_WindowEn_Pos); }

constexpr uint8_t LCD_LCDC_WinTileMap_Pos = 6;
constexpr uint8_t LCD_LCDC_WinTileMap_Msk = 1 << LCD_LCDC_WinTileMap_Pos;
constexpr uint8_t LCD_LCDC_WinTileMap(bool value) {
    return LCD_LCDC_WinTileMap_Msk & (value << LCD_LCDC_WinTileMap_Pos);
}

constexpr uint8_t LCD_LCDC_LCDEn_Pos = 7;
constexpr uint8_t LCD_LCDC_LCDEn_Msk = 1 << LCD_LCDC_LCDEn_Pos;
constexpr uint8_t LCD_LCDC_LCDEn(bool value) { return LCD_LCDC_LCDEn_Msk & (value << LCD_LCDC_LCDEn_Pos); }

constexpr uint8_t LCD_STAT_PPUMode_Pos = 0;
constexpr uint8_t LCD_STAT_PPUMode_Msk = 3 << LCD_STAT_PPUMode_Pos;
constexpr uint8_t LCD_STAT_PPUMode(bool value) { return LCD_STAT_PPUMode_Msk & (value << LCD_STAT_PPUMode_Pos); }

constexpr uint8_t LCD_STAT_LYCeLY_Pos = 2;
constexpr uint8_t LCD_STAT_LYCeLY_Msk = 1 << LCD_STAT_LYCeLY_Pos;
constexpr uint8_t LCD_STAT_LYCeLY(bool value) { return LCD_STAT_LYCeLY_Msk & (value << LCD_STAT_LYCeLY_Pos); }

constexpr uint8_t LCD_STAT_M0Int_Pos = 3;
constexpr uint8_t LCD_STAT_M0Int_Msk = 1 << LCD_STAT_M0Int_Pos;
constexpr uint8_t LCD_STAT_M0Int(bool value) { return LCD_STAT_M0Int_Msk & (value << LCD_STAT_M0Int_Pos); }

constexpr uint8_t LCD_STAT_M1Int_Pos = 4;
constexpr uint8_t LCD_STAT_M1Int_Msk = 1 << LCD_STAT_M1Int_Pos;
constexpr uint8_t LCD_STAT_M1Int(bool value) { return LCD_STAT_M1Int_Msk & (value << LCD_STAT_M1Int_Pos); }

constexpr uint8_t LCD_STAT_M2Int_Pos = 5;
constexpr uint8_t LCD_STAT_M2Int_Msk = 1 << LCD_STAT_M2Int_Pos;
constexpr uint8_t LCD_STAT_M2Int(bool value) { return LCD_STAT_M2Int_Msk & (value << LCD_STAT_M2Int_Pos); }

constexpr uint8_t LCD_STAT_LYCInt_Pos = 6;
constexpr uint8_t LCD_STAT_LYCInt_Msk = 1 << LCD_STAT_LYCInt_Pos;
constexpr uint8_t LCD_STAT_LYCInt(bool value) { return LCD_STAT_LYCInt_Msk & (value << LCD_STAT_LYCInt_Pos); }

/************************************/
/* Timer                            */
/************************************/
struct TimerRegs {
    uint8_t DIV;  /**< Divider register */
    uint8_t TIMA; /**< Timer counter */
    uint8_t TMA;  /**< Timer modulo */
    uint8_t TAC;  /**< Timer control */
};

constexpr uint8_t Timer_TAC_ClockSel_Pos = 0;
constexpr uint8_t Timer_TAC_ClockSel_Msk = 3 << Timer_TAC_ClockSel_Pos;
constexpr uint8_t Timer_TAC_ClockSel(bool value) { return Timer_TAC_ClockSel_Msk & (value << Timer_TAC_ClockSel_Pos); }

constexpr uint8_t Timer_TAC_En_Pos = 2;
constexpr uint8_t Timer_TAC_En_Msk = 1 << Timer_TAC_En_Pos;
constexpr uint8_t Timer_TAC_En(bool value) { return Timer_TAC_En_Msk & (value << Timer_TAC_En_Pos); }

/************************************/
/* Interrupts                       */
/************************************/
struct InterruptRegs {
    uint8_t IE;  /**< Interrupt enable */
    uint8_t IF; /**< Interrupt flag */
};

}  // namespace gb