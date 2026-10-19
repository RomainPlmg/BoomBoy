#include "Window.hpp"

#include <SDL3/SDL.h>

#include <cassert>

#include "Framebuffer.hpp"
#include "Log.hpp"
#include "Window.hpp"

namespace gb {

struct Color {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{255};
};

static std::vector<Color> colorPalette = {
    {224, 248, 208, 255},
    {136, 192, 112, 255},
    {52, 104, 86, 255},
    {8, 24, 32, 255},
};

static SDL_FRect computeDestRect(int windowWidth, int windowHeight, int textureWidth, int textureHeight) {
    const float windowAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    const float textureAspect = static_cast<float>(textureWidth) / static_cast<float>(textureHeight);

    SDL_FRect dest;

    if (windowAspect > textureAspect) {
        dest.h = static_cast<float>(windowHeight);
        dest.w = dest.h * textureAspect;
        dest.x = (static_cast<float>(windowWidth) - dest.w) * 0.5f;
        dest.y = 0.0f;
    } else {
        dest.w = static_cast<float>(windowWidth);
        dest.h = dest.w / textureAspect;
        dest.x = 0.0f;
        dest.y = (static_cast<float>(windowHeight) - dest.h) * 0.5f;
    }

    return dest;
}

Window::Window(const WindowProperties& props) {
    m_window = SDL_CreateWindow(props.title.c_str(), static_cast<int>(props.width), static_cast<int>(props.height),
                                SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!m_window) {
        LOG_FATAL("{}", SDL_GetError());
        std::abort();
    }

    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) {
        LOG_FATAL("{}", SDL_GetError());
        std::abort();
    }

    if (!SDL_SetRenderVSync(m_renderer, 1)) { LOG_ERROR("{}", SDL_GetError()); }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (!m_texture) {
        LOG_FATAL("{}", SDL_GetError());
        std::abort();
    }
    SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
    m_buffer.resize(160 * 144);

    LOG_DEBUG("Window init successful!");
}

Window::~Window() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);

    LOG_DEBUG("Window destroy successful!");
}

void Window::pollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) m_open = false;
    }
}

void Window::present(const Framebuffer& fb) {
    auto pixels = fb.data();

    assert(fb.data().size() == m_buffer.size() && "Framebuffer and destination texture don't have the same size.");

    for (size_t i = 0; i < pixels.size(); ++i) { m_buffer[i] = std::bit_cast<uint32_t>(colorPalette[pixels[i]]); }

    SDL_UpdateTexture(m_texture, nullptr, m_buffer.data(), 160 * sizeof(uint32_t));

    int windowW, windowH;
    SDL_GetRenderOutputSize(m_renderer, &windowW, &windowH);
    SDL_FRect dest = computeDestRect(windowW, windowH, 160, 144);

    SDL_RenderClear(m_renderer);
    SDL_RenderTexture(m_renderer, m_texture, nullptr, &dest);
    SDL_RenderPresent(m_renderer);
}

}  // namespace gb