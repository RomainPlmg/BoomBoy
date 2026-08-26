#include "core/Log.hpp"
#include "components/Cartridge.hpp"

int main(int argc, char** argv) {
    logger::init();
    logger::setLevel(logger::Level::Trace);

    if (argc < 2) {
        LOG_FATAL("Missing ROM file...");
        return EXIT_FAILURE;
    }

    Cartridge::load(argv[1]);

    return EXIT_SUCCESS;
}