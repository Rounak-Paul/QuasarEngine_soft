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

void* Renderer::begin_frame(int& pitch) {
    if (framebuffer) return framebuffer; // Already locked
    void* pixels = nullptr;
    if (!SDL_LockTexture(textures[currentBuffer], nullptr, &pixels, &pitch)) {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError() << "\n";
        return nullptr;
    }
    framebuffer = static_cast<uint32_t*>(pixels);
    framebufferPitch = pitch / sizeof(uint32_t); // convert to pixels
    pitch = framebufferPitch;
    return framebuffer;
}

void Renderer::end_frame() {
    if (framebuffer) {
        SDL_UnlockTexture(textures[currentBuffer]);
        framebuffer = nullptr;
    }
    SDL_RenderClear(sdlRenderer);
    SDL_RenderTextureRotated(sdlRenderer, textures[currentBuffer], nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
    SDL_RenderPresent(sdlRenderer);
    currentBuffer = (currentBuffer + 1) % FRAMEBUFFERS;
}

void Renderer::draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    if (!framebuffer) {
        std::cerr << "drawLine called without framebuffer lock\n";
        return;
    }

    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);
    int error2 = 0;
    int derror2 = dy * 2;
    int y = y0;
    int ystep = (y1 > y0) ? 1 : -1;

    for (int x = x0; x <= x1; ++x) {
        if (steep) {
            // Coordinates were swapped: draw at (y, x)
            if (y >= 0 && y < WIDTH && x >= 0 && x < HEIGHT)
                framebuffer[x * framebufferPitch + y] = color;
        } else {
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
                framebuffer[y * framebufferPitch + x] = color;
        }

        error2 += derror2;
        if (error2 > dx * 2) {
            y += ystep;
            error2 -= dx * 2;
        }
    }
}

void Renderer::draw_triangle(Vec2i t0, Vec2i t1, Vec2i t2, uint32_t color) {
    if (!framebuffer) {
        std::cerr << "draw_triangle called without framebuffer lock\n";
        return;
    }
    
    // Skip degenerate triangles (all points on same horizontal line)
    if (t0.y == t1.y && t0.y == t2.y) return;
    
    // Sort vertices by y-coordinate (t0.y <= t1.y <= t2.y)
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    
    int total_height = t2.y - t0.y;
    
    for (int i = 0; i < total_height; i++) {
        // Determine if we're in the second half of the triangle
        bool second_half = i > t1.y - t0.y || t1.y == t0.y;
        int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
        
        // Calculate interpolation parameters
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height;
        
        // Calculate the two x-coordinates for this scanline
        Vec2i A = t0 + (t2 - t0) * alpha;
        Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
        
        // Ensure A.x <= B.x for left-to-right drawing
        if (A.x > B.x) std::swap(A, B);
        
        // Draw horizontal line from A.x to B.x at y = t0.y + i
        int y = t0.y + i;
        
        // Bounds checking for y coordinate
        if (y < 0 || y >= HEIGHT) continue;
        
        for (int j = A.x; j <= B.x; j++) {
            // Bounds checking for x coordinate
            if (j >= 0 && j < WIDTH) {
                framebuffer[y * framebufferPitch + j] = color;
            }
        }
    }
}
