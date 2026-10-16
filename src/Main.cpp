#include <atomic>
#include <iostream>
#include <thread>

#include "Cartridge.hpp"
#include "GameBoy.hpp"
#include "Log.hpp"
#include "RomFile.hpp"
#include "Window.hpp"
#include "Framebuffer.hpp"

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

    std::atomic running = true;
    gb::GameBoy gameBoy(std::move(cartridgeExp.value()));
    gameBoy.serialLink().attachCallback(serialLinkCB);
    std::thread emulThread([&] {
        while (running) { gameBoy.step(); };
    });

    gb::Window window;
    while (!window.shouldClose()) {
        window.pollEvents();
        window.present(gameBoy.ppu().framebuffer());
    }
    running = false;

    emulThread.join();

    return EXIT_SUCCESS;
}