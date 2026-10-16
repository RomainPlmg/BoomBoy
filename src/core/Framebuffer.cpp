#include "Framebuffer.hpp"

#include <algorithm>
#include <cassert>

namespace gb {

Framebuffer::Framebuffer(int width, int height)
    : m_width(width), m_height(height), m_pixels(static_cast<size_t>(width) * height, 0) {}

void Framebuffer::setPixel(int x, int y, uint8_t colorIdx) {
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
    m_pixels[index(x, y)] = colorIdx;
}

uint8_t Framebuffer::getPixel(int x, int y) {
    assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
    return m_pixels[index(x, y)];
}

void Framebuffer::clear(uint8_t colorIdx) { std::fill(m_pixels.begin(), m_pixels.end(), colorIdx); }

}  // namespace gb