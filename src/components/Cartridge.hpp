#pragma once

#include <filesystem>
#include <memory>
#include <string_view>
#include <unordered_map>

static const std::unordered_map<uint8_t, uint32_t> RAM_CAPACITY = {
    {0, 0}, {1, 0}, {2, 8}, {3, 32}, {4, 128}, {5, 64},
};

static const std::unordered_map<uint8_t, std::string> CARTRIDGE_TYPES = {
    {0x00, "ROM ONLY"},
    {0x01, "MBC1"},
    {0x02, "MBC1+RAM"},
    {0x03, "MBC1+RAM+BATTERY"},
    {0x05, "MBC2"},
    {0x06, "MBC2+BATTERY"},
    {0x08, "ROM+RAM 1"},
    {0x09, "ROM+RAM+BATTERY 1"},
    {0x0B, "MMM01"},
    {0x0C, "MMM01+RAM"},
    {0x0D, "MMM01+RAM+BATTERY"},
    {0x0F, "MBC3+TIMER+BATTERY"},
    {0x10, "MBC3+TIMER+RAM+BATTERY 2"},
    {0x11, "MBC3"},
    {0x12, "MBC3+RAM 2"},
    {0x13, "MBC3+RAM+BATTERY 2"},
    {0x19, "MBC5"},
    {0x1A, "MBC5+RAM"},
    {0x1B, "MBC5+RAM+BATTERY"},
    {0x1C, "MBC5+RUMBLE"},
    {0x1D, "MBC5+RUMBLE+RAM"},
    {0x1E, "MBC5+RUMBLE+RAM+BATTERY"},
    {0x20, "MBC6"},
    {0x22, "MBC7+SENSOR+RUMBLE+RAM+BATTERY"},
};

static const std::unordered_map<uint8_t, std::string> LICENSEE_CODES = {
    {0x00, "None"},
    {0x01, "Nintendo"},
    {0x08, "Capcom"},
    {0x09, "HOT-B"},
    {0x0A, "Jaleco"},
    {0x0B, "Coconuts Japan"},
    {0x0C, "Elite Systems"},
    {0x13, "EA (Electronic Arts)"},
    {0x18, "Hudson Soft"},
    {0x19, "ITC Entertainment"},
    {0x1A, "Yanoman"},
    {0x1D, "Japan Clary"},
    {0x1F, "Virgin Games Ltd.3"},
    {0x24, "PCM Complete"},
    {0x25, "San-X"},
    {0x28, "Kemco"},
    {0x29, "SETA Corporation"},
    {0x30, "Infogrames5"},
    {0x31, "Nintendo"},
    {0x32, "Bandai"},
    {0x34, "Konami"},
    {0x35, "HectorSoft"},
    {0x38, "Capcom"},
    {0x39, "Banpresto"},
    {0x3C, "Entertainment Interactive (stub)"},
    {0x3E, "Gremlin"},
    {0x41, "Ubi Soft1"},
    {0x42, "Atlus"},
    {0x44, "Malibu Interactive"},
    {0x46, "Angel"},
    {0x47, "Spectrum HoloByte"},
    {0x49, "Irem"},
    {0x4A, "Virgin Games Ltd.3"},
    {0x4D, "Malibu Interactive"},
    {0x4F, "U.S. Gold"},
    {0x50, "Absolute"},
    {0x51, "Acclaim Entertainment"},
    {0x52, "Activision"},
    {0x53, "Sammy USA Corporation"},
    {0x54, "GameTek"},
    {0x55, "Park Place15"},
    {0x56, "LJN"},
    {0x57, "Matchbox"},
    {0x59, "Milton Bradley Company"},
    {0x5A, "Mindscape"},
    {0x5B, "Romstar"},
    {0x5C, "Naxat Soft16"},
    {0x5D, "Tradewest"},
    {0x60, "Titus Interactive"},
    {0x61, "Virgin Games Ltd.3"},
    {0x67, "Ocean Software"},
    {0x69, "EA (Electronic Arts)"},
    {0x6E, "Elite Systems"},
    {0x6F, "Electro Brain"},
    {0x70, "Infogrames5"},
    {0x71, "Interplay Entertainment"},
    {0x72, "Broderbund"},
    {0x73, "Sculptured Software6"},
    {0x75, "The Sales Curve Limited7"},
    {0x78, "THQ"},
    {0x79, "Accolade8"},
    {0x7A, "Triffix Entertainment"},
    {0x7C, "MicroProse"},
    {0x7F, "Kemco"},
    {0x80, "Misawa Entertainment"},
    {0x83, "LOZC G."},
    {0x86, "Tokuma Shoten"},
    {0x8B, "Bullet-Proof Software2"},
    {0x8C, "Vic Tokai Corp.17"},
    {0x8E, "Ape Inc.18"},
    {0x8F, "I'Max19"},
    {0x91, "Chunsoft Co.9"},
    {0x92, "Video System"},
    {0x93, "Tsubaraya Productions"},
    {0x95, "Varie"},
    {0x96, "Yonezawa10/S'Pal"},
    {0x97, "Kemco"},
    {0x99, "Arc"},
    {0x9A, "Nihon Bussan"},
    {0x9B, "Tecmo"},
    {0x9C, "Imagineer"},
    {0x9D, "Banpresto"},
    {0x9F, "Nova"},
    {0xA1, "Hori Electric"},
    {0xA2, "Bandai"},
    {0xA4, "Konami"},
    {0xA6, "Kawada"},
    {0xA7, "Takara"},
    {0xA9, "Technos Japan"},
    {0xAA, "Broderbund"},
    {0xAC, "Toei Animation"},
    {0xAD, "Toho"},
    {0xAF, "Namco"},
    {0xB0, "Acclaim Entertainment"},
    {0xB1, "ASCII Corporation or Nexsoft"},
    {0xB2, "Bandai"},
    {0xB4, "Square Enix"},
    {0xB6, "HAL Laboratory"},
    {0xB7, "SNK"},
    {0xB9, "Pony Canyon"},
    {0xBA, "Culture Brain"},
    {0xBB, "Sunsoft"},
    {0xBD, "Sony Imagesoft"},
    {0xBF, "Sammy Corporation"},
    {0xC0, "Taito"},
    {0xC2, "Kemco"},
    {0xC3, "Square"},
    {0xC4, "Tokuma Shoten"},
    {0xC5, "Data East"},
    {0xC6, "Tonkin House"},
    {0xC8, "Koei"},
    {0xC9, "UFL"},
    {0xCA, "Ultra Games"},
    {0xCB, "VAP, Inc."},
    {0xCC, "Use Corporation"},
    {0xCD, "Meldac"},
    {0xCE, "Pony Canyon"},
    {0xCF, "Angel"},
    {0xD0, "Taito"},
    {0xD1, "SOFEL (Software Engineering Lab)"},
    {0xD2, "Quest"},
    {0xD3, "Sigma Enterprises"},
    {0xD4, "ASK Kodansha Co."},
    {0xD6, "Naxat Soft16"},
    {0xD7, "Copya System"},
    {0xD9, "Banpresto"},
    {0xDA, "Tomy"},
    {0xDB, "LJN"},
    {0xDD, "Nippon Computer Systems"},
    {0xDE, "Human Ent."},
    {0xDF, "Altron"},
    {0xE0, "Jaleco"},
    {0xE1, "Towa Chiki"},
    {0xE2, "Yutaka # Needs more info"},
    {0xE3, "Varie"},
    {0xE5, "Epoch"},
    {0xE7, "Athena"},
    {0xE8, "Asmik Ace Entertainment"},
    {0xE9, "Natsume"},
    {0xEA, "King Records"},
    {0xEB, "Atlus"},
    {0xEC, "Epic/Sony Records"},
    {0xEE, "IGS"},
    {0xF0, "A Wave"},
    {0xF3, "Extreme Entertainment"},
    {0xFF, "LJN"},
};

static const std::unordered_map<uint8_t, std::string> NEW_LICENSEE_CODES = {
    {0x00, "None"},
    {0x01, "Nintendo R&D"},
    {0x08, "Capcom"},
    {0x13, "EA"},
    {0x18, "Hudson Soft"},
    {0x19, "B-AI"},
    {0x20, "KSS"},
    {0x22, "Planning Office WADA"},
    {0x24, "PCM Complete"},
    {0x25, "San-X"},
    {0x28, "Kemco"},
    {0x29, "Seta Corporation"},
    {0x30, "Viacom"},
    {0x31, "Nintendo"},
    {0x32, "Bandai"},
    {0x33, "Ocean Software/Acclaim Entertainment"},
    {0x34, "Konami"},
    {0x35, "HectorSoft"},
    {0x37, "Taito"},
    {0x38, "Hudson Soft"},
    {0x39, "Banpresto"},
    {0x41, "Ubisoft"},
    {0x42, "Atlus"},
    {0x44, "Malibu Interactive"},
    {0x46, "Angel"},
    {0x47, "Bullet-Proof Software"},
    {0x49, "Irem"},
    {0x50, "Absolute"},
    {0x51, "Acclaim Entertainment"},
    {0x52, "Activision"},
    {0x53, "Sammy USA Corporation"},
    {0x54, "Konami"},
    {0x55, "Hi tech Entertainment"},
    {0x56, "LJN"},
    {0x57, "Matchbox"},
    {0x58, "Mattel"},
    {0x59, "Milton Bradley Company"},
    {0x60, "Titus Interactive"},
    {0x61, "Virgin Game Ltd."},
    {0x64, "Lucasfilm Games"},
    {0x67, "Ocean Software"},
    {0x69, "EA"},
    {0x70, "Infogrames"},
    {0x71, "Interplay Entertainment"},
    {0x72, "Broderbund"},
    {0x73, "Sculptured Software"},
    {0x75, "The Sales Curve Limited"},
    {0x78, "THQ"},
    {0x79, "Accolade"},
    {0x80, "Misawa Entertainment"},
    {0x83, "lozc"},
    {0x86, "Tokuma Shoten"},
    {0x87, "Tsukuda Original"},
    {0x91, "Chunsoft Co."},
    {0x92, "Video System"},
    {0x93, "Ocean Software/Acclaim Entertainment"},
    {0x95, "Varie"},
    {0x96, "Yonezawa/s\'pal"},
    {0x97, "Kaneko"},
    {0x99, "Pack-In-Video"},
    {0xA4, "Konami (Yu-Gi-Oh!)"},
};

struct CartridgeHeader {
    uint8_t entry[4];
    uint8_t logo[48];
    uint8_t title[16];
    uint16_t newLicenseeCode;
    uint8_t sgb;
    uint8_t type;
    uint8_t romSize;
    uint8_t ramSize;
    uint8_t destCode;
    uint8_t licenseeCode;
    uint8_t version;
    uint8_t checksum;
    uint16_t globalChecksum;

    std::string_view titleStr() const {
        std::string_view sv{reinterpret_cast<const char*>(title), sizeof(title)};
        return sv.substr(0, sv.find('\0'));
    }

    std::string typeStr() const {
        if (CARTRIDGE_TYPES.contains(type)) return CARTRIDGE_TYPES.at(type);
        return "Unknown";
    };

    std::string ramSizeStr() const {
        if (RAM_CAPACITY.contains(type)) return std::to_string(RAM_CAPACITY.at(ramSize)) + " KiB";
        return std::string("Unknown");
    }
};

class Cartridge {
   public:
    static std::unique_ptr<Cartridge> load(const std::filesystem::path& path);

    [[nodiscard]] const CartridgeHeader& header() const { return m_header; }

   private:
    Cartridge() = default;

    std::vector<std::byte> m_data;
    CartridgeHeader m_header;
};