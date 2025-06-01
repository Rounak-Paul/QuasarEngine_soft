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

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        int pitch;
        if (!renderer.begin_frame(pitch)) continue;

        renderer.draw_line(13, 20, 80, 40, 0xFFFF0000);  // white
        renderer.draw_line(20, 13, 40, 80, 0xFFFF0000);  // red
        renderer.draw_line(80, 40, 13, 20, 0xFFFF0000);  // red

        renderer.end_frame();

        frameCount++;
        uint32_t now = SDL_GetTicks();
        if (now - lastTime >= 1000) {
            char title[128];
            snprintf(title, sizeof(title), "QuasarSoft - FPS: %d", frameCount);
            SDL_SetWindowTitle(renderer.get_window(), title);
            frameCount = 0;
            lastTime = now;
        }

        SDL_Delay(1);
    }
}

void Application::shutdown() {
    renderer.shutdown();
}