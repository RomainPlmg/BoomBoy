#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
union SDL_Event;

namespace gb {

class Framebuffer;

struct WindowProperties {
    uint32_t width = 800;
    uint32_t height = 720;
    std::string title = "BoomBoy";
};

class Window {
   public:
    Window(const WindowProperties& props = {});
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    void pollEvents();

    void present(const Framebuffer& fb);

    [[nodiscard]] bool shouldClose() const { return !m_open; }
    [[nodiscard]] SDL_Window* window() const { return m_window; }
    [[nodiscard]] SDL_Renderer* renderer() const { return m_renderer; }

   private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture;
    WindowProperties m_props;
    bool m_open = true;

    std::vector<uint32_t> m_buffer;
};

}  // namespace gb