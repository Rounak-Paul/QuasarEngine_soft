#include "Application.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <Resources/Model.h>

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

    Model model = Model("../Assets/teapot.obj");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        int pitch;
        if (!renderer.begin_frame(pitch)) continue;

        for (int i=0; i<model.nfaces(); i++) { 
            std::vector<int> face = model.face(i); 
            for (int j=0; j<3; j++) { 
                Vec3f v0 = model.vert(face[j]); 
                Vec3f v1 = model.vert(face[(j+1)%3]); 
                int x0 = (v0.x+1.)*WIDTH/2.; 
                int y0 = (v0.y+1.)*HEIGHT/2.; 
                int x1 = (v1.x+1.)*WIDTH/2.; 
                int y1 = (v1.y+1.)*HEIGHT/2.; 
                renderer.draw_line(x0, y0, x1, y1, 0xFFFF0000); 
            } 
        }

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