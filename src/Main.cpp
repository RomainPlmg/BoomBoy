#include "Cartridge.hpp"
#include "cartridge/RomFile.hpp"
#include "core/Log.hpp"

int main(int argc, char** argv) {
    gb::logger::init();
    gb::logger::setLevel(gb::logger::Level::Trace);

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

    return EXIT_SUCCESS;
}