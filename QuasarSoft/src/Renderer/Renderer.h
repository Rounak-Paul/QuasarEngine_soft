#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <vector>

#define WIDTH 800
#define HEIGHT 600
#define FRAMEBUFFERS 2

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void shutdown();

    void* begin_frame(int& pitch);
    void end_frame();

    // New line drawing method
    void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

    SDL_Window* get_window() const { return window; }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    std::array<SDL_Texture*, FRAMEBUFFERS> textures{};
    int currentBuffer = 0;

    uint32_t* framebuffer = nullptr;
    int framebufferPitch = 0; // in pixels
};