#include "Timer.hpp"

#include <chrono>

namespace {

uint64_t now() {
    using namespace std::chrono;
    return static_cast<uint64_t>(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
}

}  // namespace

void Timer::tick() {
    const uint64_t t = now();

    // First tick
    if (m_last == 0) {
        m_last = t;
        m_delta = 0.f;
        m_unscaled = 0.f;
        return;
    }

    m_unscaled = static_cast<float>(t - m_last) * 1e-9f;
    m_delta = m_unscaled * m_scale;
    m_elapsed += m_delta;
    m_last = t;
    m_frame++;
}
