#include <iostream>
#include <thread>

#include "Cartridge.hpp"
#include "GameBoy.hpp"
#include "Log.hpp"
#include "RomFile.hpp"
#include "Window.hpp"

constexpr auto FRAME_DURATION = std::chrono::nanoseconds((1'000'000'000LL * gb::CYCLES_PER_FRAME) / gb::CPU_FREQ);

static void serialLinkCB(char c) { std::cout << c; }

int main(const int argc, char** argv) {
    gb::logger::init();
    gb::logger::setLevel(gb::logger::Level::Info);

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

    auto bootRom = gb::BootRom::load("../vendor/bootix_dmg.bin");

    gb::GameBoy gameBoy(std::move(cartridgeExp.value()), std::move(bootRom.value()));

    gb::Window window;
    using clock = std::chrono::steady_clock;
    auto nextFrameTime = clock::now() + FRAME_DURATION;

    while (!window.shouldClose()) {
        window.pollEvents();

        uint64_t currentFrame = gameBoy.ppu().frameCount();
        while (gameBoy.ppu().frameCount() == currentFrame) gameBoy.step();

        window.present(gameBoy.ppu().framebuffer());

        std::this_thread::sleep_until(nextFrameTime);
        nextFrameTime += FRAME_DURATION;
    }

    return EXIT_SUCCESS;
}