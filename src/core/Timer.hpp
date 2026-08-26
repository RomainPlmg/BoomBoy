#pragma once

#include <cstdint>

class Timer {
   public:
    Timer() = default;

    void tick();

    // Getters
    [[nodiscard]] float delta() const noexcept { return m_delta; }
    [[nodiscard]] float unscaled() const noexcept { return m_unscaled; }
    [[nodiscard]] double elapsed() const noexcept { return m_elapsed; }
    [[nodiscard]] uint64_t frame() const noexcept { return m_frame; }
    [[nodiscard]] float scale() const noexcept { return m_scale; }

    // Setters
    void setScale(float scale) { m_scale = scale; }

   private:
    uint64_t m_last = 0;
    float m_delta = 0.f;
    float m_unscaled = 0.f;
    double m_elapsed = 0.0;
    uint64_t m_frame = 0;
    float m_scale = 1.f;
};