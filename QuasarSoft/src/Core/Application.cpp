#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>

Application::Application() {}

Application::~Application() {
    shutdown();
}

bool Application::init() {
    if (!renderer.init()) return false;
    lastTime = SDL_GetTicks();
    return true;
}

void Application::run() {
    SDL_Event event;

    std::vector<Vertex> vertices = {
        {400, 100, 0xFFFF0000}, // red
        {600, 500, 0xFF00FF00}, // green
        {200, 500, 0xFF0000FF}, // blue
    };

    std::vector<uint32_t> indices = {0, 1, 2};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        renderer.drawMesh(vertices, indices);

        frameCount++;
        uint32_t now = SDL_GetTicks();
        if (now - lastTime >= 1000) {
            char title[128];
            snprintf(title, sizeof(title), "QuasarSoft - FPS: %d", frameCount);
            SDL_SetWindowTitle(renderer.getWindow(), title);
            frameCount = 0;
            lastTime = now;
        }

        SDL_Delay(1);
    }
}

void Application::shutdown() {
    renderer.shutdown();
}