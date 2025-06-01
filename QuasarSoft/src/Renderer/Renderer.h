#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <vector>
#include "Vertex.h"

#define WIDTH 800
#define HEIGHT 600
#define FRAMEBUFFERS 2

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();
    void shutdown();

    void drawMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

    SDL_Window* getWindow() const { return window; };

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    std::array<SDL_Texture*, FRAMEBUFFERS> textures{};
    int currentBuffer = 0;
    
    void* lockCurrent(int& pitch);
    void unlockAndPresent();
};