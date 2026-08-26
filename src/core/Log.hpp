#pragma once

#include <format>
#include <string_view>
#include <utility>

namespace logger {

enum class Level { Trace, Debug, Info, Warn, Err, Fatal };

void init();
void shutdown();
void setLevel(Level);
Level level() noexcept;

void write(Level, std::string_view msg) noexcept;

template <typename... Args>
void logf(Level lvl, std::format_string<Args...> fmt, Args&&... args) {
    if (lvl < level()) return;
    write(lvl, std::format(fmt, std::forward<Args>(args)...));
}

}  // namespace logger

#define LOG_TRACE(...) ::logger::logf(::logger::Level::Trace, __VA_ARGS__)
#define LOG_DEBUG(...) ::logger::logf(::logger::Level::Debug, __VA_ARGS__)
#define LOG_INFO(...) ::logger::logf(::logger::Level::Info, __VA_ARGS__)
#define LOG_WARN(...) ::logger::logf(::logger::Level::Warn, __VA_ARGS__)
#define LOG_ERROR(...) ::logger::logf(::logger::Level::Err, __VA_ARGS__)
#define LOG_FATAL(...) ::logger::logf(::logger::Level::Fatal, __VA_ARGS__)

#ifdef LOG_STRIP_TRACE
#undef LOG_TRACE
#define LOG_TRACE(...) ((void)0)
#endif