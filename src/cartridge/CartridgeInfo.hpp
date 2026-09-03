#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace gb::cart {

constexpr std::array<uint8_t, 6> RAM_CAPACITY{
    0, 0, 8, 32, 128, 64,
};

constexpr std::array<std::string_view, 0x23> CARTRIDGE_TYPES{
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "Unknown",
    "MBC2",
    "MBC2+BATTERY",
    "Unknown",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "Unknown",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "Unknown",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "Unknown",
    "MBC6",
    "Unknown",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

constexpr std::string_view LicenseeCode(const uint8_t code) {
    switch (code) {
        case 0x00: return "None";
        case 0x01: return "Nintendo";
        case 0x08: return "Capcom";
        case 0x09: return "HOT-B";
        case 0x0A: return "Jaleco";
        case 0x0B: return "Coconuts Japan";
        case 0x0C: return "Elite Systems";
        case 0x13: return "EA (Electronic Arts)";
        case 0x18: return "Hudson Soft";
        case 0x19: return "ITC Entertainment";
        case 0x1A: return "Yanoman";
        case 0x1D: return "Japan Clary";
        case 0x1F: return "Virgin Games Ltd.3";
        case 0x24: return "PCM Complete";
        case 0x25: return "San-X";
        case 0x28: return "Kemco";
        case 0x29: return "SETA Corporation";
        case 0x30: return "Infogrames5";
        case 0x31: return "Nintendo";
        case 0x32: return "Bandai";
        case 0x34: return "Konami";
        case 0x35: return "HectorSoft";
        case 0x38: return "Capcom";
        case 0x39: return "Banpresto";
        case 0x3C: return "Entertainment Interactive (stub)";
        case 0x3E: return "Gremlin";
        case 0x41: return "Ubi Soft1";
        case 0x42: return "Atlus";
        case 0x44: return "Malibu Interactive";
        case 0x46: return "Angel";
        case 0x47: return "Spectrum HoloByte";
        case 0x49: return "Irem";
        case 0x4A: return "Virgin Games Ltd.3";
        case 0x4D: return "Malibu Interactive";
        case 0x4F: return "U.S. Gold";
        case 0x50: return "Absolute";
        case 0x51: return "Acclaim Entertainment";
        case 0x52: return "Activision";
        case 0x53: return "Sammy USA Corporation";
        case 0x54: return "GameTek";
        case 0x55: return "Park Place15";
        case 0x56: return "LJN";
        case 0x57: return "Matchbox";
        case 0x59: return "Milton Bradley Company";
        case 0x5A: return "Mindscape";
        case 0x5B: return "Romstar";
        case 0x5C: return "Naxat Soft16";
        case 0x5D: return "Tradewest";
        case 0x60: return "Titus Interactive";
        case 0x61: return "Virgin Games Ltd.3";
        case 0x67: return "Ocean Software";
        case 0x69: return "EA (Electronic Arts)";
        case 0x6E: return "Elite Systems";
        case 0x6F: return "Electro Brain";
        case 0x70: return "Infogrames5";
        case 0x71: return "Interplay Entertainment";
        case 0x72: return "Broderbund";
        case 0x73: return "Sculptured Software6";
        case 0x75: return "The Sales Curve Limited7";
        case 0x78: return "THQ";
        case 0x79: return "Accolade8";
        case 0x7A: return "Triffix Entertainment";
        case 0x7C: return "MicroProse";
        case 0x7F: return "Kemco";
        case 0x80: return "Misawa Entertainment";
        case 0x83: return "LOZC G.";
        case 0x86: return "Tokuma Shoten";
        case 0x8B: return "Bullet-Proof Software2";
        case 0x8C: return "Vic Tokai Corp.17";
        case 0x8E: return "Ape Inc.18";
        case 0x8F: return "I'Max19";
        case 0x91: return "Chunsoft Co.9";
        case 0x92: return "Video System";
        case 0x93: return "Tsubaraya Productions";
        case 0x95: return "Varie";
        case 0x96: return "Yonezawa10/S'Pal";
        case 0x97: return "Kemco";
        case 0x99: return "Arc";
        case 0x9A: return "Nihon Bussan";
        case 0x9B: return "Tecmo";
        case 0x9C: return "Imagineer";
        case 0x9D: return "Banpresto";
        case 0x9F: return "Nova";
        case 0xA1: return "Hori Electric";
        case 0xA2: return "Bandai";
        case 0xA4: return "Konami";
        case 0xA6: return "Kawada";
        case 0xA7: return "Takara";
        case 0xA9: return "Technos Japan";
        case 0xAA: return "Broderbund";
        case 0xAC: return "Toei Animation";
        case 0xAD: return "Toho";
        case 0xAF: return "Namco";
        case 0xB0: return "Acclaim Entertainment";
        case 0xB1: return "ASCII Corporation or Nexsoft";
        case 0xB2: return "Bandai";
        case 0xB4: return "Square Enix";
        case 0xB6: return "HAL Laboratory";
        case 0xB7: return "SNK";
        case 0xB9: return "Pony Canyon";
        case 0xBA: return "Culture Brain";
        case 0xBB: return "Sunsoft";
        case 0xBD: return "Sony Imagesoft";
        case 0xBF: return "Sammy Corporation";
        case 0xC0: return "Taito";
        case 0xC2: return "Kemco";
        case 0xC3: return "Square";
        case 0xC4: return "Tokuma Shoten";
        case 0xC5: return "Data East";
        case 0xC6: return "Tonkin House";
        case 0xC8: return "Koei";
        case 0xC9: return "UFL";
        case 0xCA: return "Ultra Games";
        case 0xCB: return "VAP, Inc.";
        case 0xCC: return "Use Corporation";
        case 0xCD: return "Meldac";
        case 0xCE: return "Pony Canyon";
        case 0xCF: return "Angel";
        case 0xD0: return "Taito";
        case 0xD1: return "SOFEL (Software Engineering Lab)";
        case 0xD2: return "Quest";
        case 0xD3: return "Sigma Enterprises";
        case 0xD4: return "ASK Kodansha Co.";
        case 0xD6: return "Naxat Soft16";
        case 0xD7: return "Copya System";
        case 0xD9: return "Banpresto";
        case 0xDA: return "Tomy";
        case 0xDB: return "LJN";
        case 0xDD: return "Nippon Computer Systems";
        case 0xDE: return "Human Ent.";
        case 0xDF: return "Altron";
        case 0xE0: return "Jaleco";
        case 0xE1: return "Towa Chiki";
        case 0xE2: return "Yutaka # Needs more info";
        case 0xE3: return "Varie";
        case 0xE5: return "Epoch";
        case 0xE7: return "Athena";
        case 0xE8: return "Asmik Ace Entertainment";
        case 0xE9: return "Natsume";
        case 0xEA: return "King Records";
        case 0xEB: return "Atlus";
        case 0xEC: return "Epic/Sony Records";
        case 0xEE: return "IGS";
        case 0xF0: return "A Wave";
        case 0xF3: return "Extreme Entertainment";
        case 0xFF: return "LJN";
        default: return "Unknown";
    }
}

constexpr std::string_view NewLicenseeCode(const uint8_t code) {
    switch (code) {
        case 0x00: return "None";
        case 0x01: return "Nintendo R&D";
        case 0x08: return "Capcom";
        case 0x13: return "EA";
        case 0x18: return "Hudson Soft";
        case 0x19: return "B-AI";
        case 0x20: return "KSS";
        case 0x22: return "Planning Office WADA";
        case 0x24: return "PCM Complete";
        case 0x25: return "San-X";
        case 0x28: return "Kemco";
        case 0x29: return "Seta Corporation";
        case 0x30: return "Viacom";
        case 0x31: return "Nintendo";
        case 0x32: return "Bandai";
        case 0x33: return "Ocean Software/Acclaim Entertainment";
        case 0x34: return "Konami";
        case 0x35: return "HectorSoft";
        case 0x37: return "Taito";
        case 0x38: return "Hudson Soft";
        case 0x39: return "Banpresto";
        case 0x41: return "Ubisoft";
        case 0x42: return "Atlus";
        case 0x44: return "Malibu Interactive";
        case 0x46: return "Angel";
        case 0x47: return "Bullet-Proof Software";
        case 0x49: return "Irem";
        case 0x50: return "Absolute";
        case 0x51: return "Acclaim Entertainment";
        case 0x52: return "Activision";
        case 0x53: return "Sammy USA Corporation";
        case 0x54: return "Konami";
        case 0x55: return "Hi tech Entertainment";
        case 0x56: return "LJN";
        case 0x57: return "Matchbox";
        case 0x58: return "Mattel";
        case 0x59: return "Milton Bradley Company";
        case 0x60: return "Titus Interactive";
        case 0x61: return "Virgin Game Ltd.";
        case 0x64: return "Lucasfilm Games";
        case 0x67: return "Ocean Software";
        case 0x69: return "EA";
        case 0x70: return "Infogrames";
        case 0x71: return "Interplay Entertainment";
        case 0x72: return "Broderbund";
        case 0x73: return "Sculptured Software";
        case 0x75: return "The Sales Curve Limited";
        case 0x78: return "THQ";
        case 0x79: return "Accolade";
        case 0x80: return "Misawa Entertainment";
        case 0x83: return "lozc";
        case 0x86: return "Tokuma Shoten";
        case 0x87: return "Tsukuda Original";
        case 0x91: return "Chunsoft Co.";
        case 0x92: return "Video System";
        case 0x93: return "Ocean Software/Acclaim Entertainment";
        case 0x95: return "Varie";
        case 0x96: return "Yonezawa/s\'pal";
        case 0x97: return "Kaneko";
        case 0x99: return "Pack-In-Video";
        case 0xA4: return "Konami (Yu-Gi-Oh!)";
        default: return "Unknown";
    }
}

}  // namespace gb::cart