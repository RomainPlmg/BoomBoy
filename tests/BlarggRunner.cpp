#include <cstdint>
#include <iostream>

#include "Cartridge.hpp"
#include "GameBoy.hpp"
#include "Log.hpp"
#include "RomFile.hpp"

constexpr uint64_t BLARGG_MAX_CYCLES = 250'000'000;

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

    std::string msg;
    bool result = false;
    gameBoy.serialLink().attachCallback([&msg, &result](char c) {
        std::cout << c;
        msg.push_back(c);
        if (msg.find("Passed") != std::string::npos) result = true;
    });

    uint32_t cycles = 0;
    while (cycles < BLARGG_MAX_CYCLES) {
        cycles += gameBoy.step();
        if (result) { return EXIT_SUCCESS; }
    }

    LOG_ERROR("Test timeout...");

    return EXIT_FAILURE;
}