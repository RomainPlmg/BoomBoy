#pragma once

#include <cstdint>
#include <span>
#include <vector>

namespace gb {

class Framebuffer {
   public:
    Framebuffer(int width, int height);

    [[nodiscard]] int width() const { return m_width; }
    [[nodiscard]] int height() const { return m_height; }

    void setPixel(int x, int y, uint8_t colorIdx);
    uint8_t getPixel(int x, int y);

    void clear(uint8_t colorIdx = 0);

    [[nodiscard]] std::span<const uint8_t> data() const { return m_pixels; }

   private:
    int m_width, m_height;
    std::vector<uint8_t> m_pixels;

    [[nodiscard]] size_t index(int x, int y) const { return static_cast<size_t>(y) * m_width + x; }
};

}  // namespace gb