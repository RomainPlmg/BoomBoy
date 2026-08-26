#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <cstdio>
#include <memory>

namespace logger {

std::atomic<Level> g_level{Level::Info};
std::atomic<spdlog::logger*> g_logger{nullptr};
std::shared_ptr<spdlog::logger> g_owner;

namespace {

constexpr spdlog::level::level_enum toSpd(Level l) noexcept {
    switch (l) {
        case Level::Trace:
            return spdlog::level::trace;
        case Level::Debug:
            return spdlog::level::debug;
        case Level::Info:
            return spdlog::level::info;
        case Level::Warn:
            return spdlog::level::warn;
        case Level::Err:
            return spdlog::level::err;
        case Level::Fatal:
            return spdlog::level::critical;
    }
    return spdlog::level::info;
}

constexpr const char* toStr(Level l) noexcept {
    switch (l) {
        case Level::Trace:
            return "trace";
        case Level::Debug:
            return "debug";
        case Level::Info:
            return "info";
        case Level::Warn:
            return "warn";
        case Level::Err:
            return "error";
        case Level::Fatal:
            return "fatal";
    }
    return "?";
}

}  // namespace

void init() {
    if (g_logger.load(std::memory_order_acquire)) {
        write(Level::Warn, "Logger already initialized.");
        return;
    }
    g_owner = spdlog::stdout_color_mt("DOOM");
    g_owner->set_pattern("%^[%l] %v%$");
    g_owner->set_level(toSpd(g_level.load(std::memory_order_relaxed)));
    g_logger.store(g_owner.get(), std::memory_order_release);
    write(Level::Debug, "Logger init successful!");
}

void shutdown() {
    g_logger.store(nullptr, std::memory_order_release);
    g_owner.reset();
    spdlog::shutdown();
}

void setLevel(Level l) {
    g_level.store(l, std::memory_order_relaxed);
    if (auto* lg = g_logger.load(std::memory_order_acquire)) lg->set_level(toSpd(l));
}

Level level() noexcept { return g_level.load(std::memory_order_relaxed); }

void write(Level lvl, std::string_view msg) noexcept {
    if (auto* lg = g_logger.load(std::memory_order_acquire)) {
        lg->log(toSpd(lvl), msg);
    } else {
        std::fprintf(stderr, "[%s]\t%.*s\n", toStr(lvl), static_cast<int>(msg.size()), msg.data());
    }
}

}  // namespace logger