#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <vector>
#include <Math/Geometry.h>

#define WIDTH 800
#define HEIGHT 600
#define FRAMEBUFFERS 3

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void shutdown();

    void* begin_frame();
    void end_frame();

    void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
    void draw_triangle(Vec2i t0, Vec2i t1, Vec2i t2, uint32_t color);

    SDL_Window* get_window() const { return window; }

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* sdl_renderer = nullptr;
    std::array<SDL_Texture*, FRAMEBUFFERS> textures{};
    int currentBuffer = 0;

    uint32_t* framebuffer = nullptr;
    int framebuffer_pitch = 0; // in pixels
};