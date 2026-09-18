#include <cstdint>

#include "Cartridge.hpp"
#include "GameBoy.hpp"
#include "Log.hpp"
#include "RomFile.hpp"

constexpr uint64_t MOONEYE_MAX_CYCLES = 50'000'000;

int main(const int argc, char** argv) {
    gb::logger::init();
    gb::logger::setLevel(gb::logger::Level::Trace);

    LOG_INFO("Run Mooneye test...");

    if (argc < 2) {
        LOG_FATAL("Missing ROM file.");
        return EXIT_FAILURE;
    }

    auto romFileExp = gb::RomFile::load(argv[1]);
    if (!romFileExp.has_value()) {
        LOG_ERROR("Failed to load ROM file.");
        return EXIT_FAILURE;
    }

    auto cartridgeExp = gb::Cartridge::create(std::move(romFileExp.value()));
    if (!cartridgeExp.has_value()) {
        LOG_ERROR("Failed to create cartridge.");
        return EXIT_FAILURE;
    }

    gb::GameBoy gameBoy(std::move(cartridgeExp.value()));

    uint32_t cycles = 0;
    while (cycles < MOONEYE_MAX_CYCLES) {
        cycles += gameBoy.step();
        if (gameBoy.cpu().currentOP() == 0x40) {
            const auto& regs = gameBoy.cpu().regs();
            return !(regs.b == 3 && regs.c == 5 && regs.d == 8 && regs.e == 13 && regs.h == 21 && regs.l == 34);
        }
    }

    return EXIT_FAILURE;
}