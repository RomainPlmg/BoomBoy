#include <iostream>

#include "Cartridge.hpp"
#include "GameBoy.hpp"
#include "Log.hpp"
#include "RomFile.hpp"

static void serialLinkCB(char c) { std::cout << c; }

int main(const int argc, char** argv) {
    gb::logger::init();
    gb::logger::setLevel(gb::logger::Level::Fatal);

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
    gameBoy.serialLink().attachCallback(serialLinkCB);

    while (true) { gameBoy.step(); }

    return EXIT_SUCCESS;
}