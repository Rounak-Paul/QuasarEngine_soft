#include "Renderer.h"
#include <iostream>

Renderer::Renderer() {}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("QuasarSoft", WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    if (!window) return false;

    sdlRenderer = SDL_CreateRenderer(window, nullptr);
    if (!sdlRenderer) return false;

    for (int i = 0; i < FRAMEBUFFERS; ++i) {
        textures[i] = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
        if (!textures[i]) return false;
    }

    return true;
}

void Renderer::shutdown() {
    for (SDL_Texture* tex : textures) {
        if (tex) SDL_DestroyTexture(tex);
    }
    if (sdlRenderer) SDL_DestroyRenderer(sdlRenderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void* Renderer::lockCurrent(int& pitch) {
    void* pixels = nullptr;
    if (!SDL_LockTexture(textures[currentBuffer], nullptr, &pixels, &pitch)) {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError() << "\n";
        return nullptr;
    }
    return pixels;
}

void Renderer::unlockAndPresent() {
    SDL_UnlockTexture(textures[currentBuffer]);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderTexture(sdlRenderer, textures[currentBuffer], nullptr, nullptr);
    SDL_RenderPresent(sdlRenderer);
    currentBuffer = (currentBuffer + 1) % FRAMEBUFFERS;
}

void Renderer::drawMesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
{
    int pitch = 0;
    uint32_t* pixels = static_cast<uint32_t*>(lockCurrent(pitch));
    if (!pixels) return;

    pitch /= sizeof(uint32_t); // Convert pitch from bytes to pixels

    auto putPixel = [&](int x, int y, uint32_t color) {
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
        pixels[y * pitch + x] = color;
    };

    auto edgeFunction = [](const Vertex& a, const Vertex& b, const Vertex& c) {
        return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    };

    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        const Vertex& v0 = vertices[indices[i]];
        const Vertex& v1 = vertices[indices[i + 1]];
        const Vertex& v2 = vertices[indices[i + 2]];

        float minX = std::min({v0.x, v1.x, v2.x});
        float minY = std::min({v0.y, v1.y, v2.y});
        float maxX = std::max({v0.x, v1.x, v2.x});
        float maxY = std::max({v0.y, v1.y, v2.y});

        int x0 = static_cast<int>(std::floor(minX));
        int y0 = static_cast<int>(std::floor(minY));
        int x1 = static_cast<int>(std::ceil(maxX));
        int y1 = static_cast<int>(std::ceil(maxY));

        float area = edgeFunction(v0, v1, v2);
        if (area == 0) continue; // Degenerate triangle

        for (int y = y0; y <= y1; ++y) {
            for (int x = x0; x <= x1; ++x) {
                Vertex p = { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f };
                float w0 = edgeFunction(v1, v2, p);
                float w1 = edgeFunction(v2, v0, p);
                float w2 = edgeFunction(v0, v1, p);

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    putPixel(x, y, v0.color); // Flat color for now
                }
            }
        }
    }

    unlockAndPresent();
}
