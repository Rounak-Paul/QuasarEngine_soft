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

// TODO: move to math library
uint32_t intensity_to_color(float intensity) {
    if (intensity < 0) intensity = 0;
    if (intensity > 1) intensity = 1;
    
    uint8_t color_value = static_cast<uint8_t>(intensity * 255);
    return 0xFF000000 | (color_value << 16) | (color_value << 8) | color_value; // ARGB format
}

void Application::run() {
    SDL_Event event;
    Model model = Model("../Assets/teapot.obj");
    Vec3f light_dir(0, 0, -1); // Light direction vector
    light_dir.normalize();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        
        if (!renderer.begin_frame()) continue;
        
        // Render all faces of the model
        for (int i = 0; i < model.nfaces(); i++) {
            std::vector<int> face = model.face(i);
            Vec2i screen_coords[3];
            Vec3f world_coords[3];
            
            // Transform vertices from world space to screen space
            for (int j = 0; j < 3; j++) {
                Vec3f v = model.vert(face[j]);
                screen_coords[j] = Vec2i((v.x + 1.) * WIDTH / 2., (v.y + 1.) * HEIGHT / 2.);
                world_coords[j] = v;
            }
            
            // Calculate face normal using cross product
            Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
            n.normalize();
            
            // Calculate lighting intensity using dot product
            float intensity = n * light_dir;
            
            // Only render faces facing towards the light (back-face culling)
            if (intensity > 0) {
                uint32_t color = intensity_to_color(intensity);
                renderer.draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2], color);
            }
        }
        
        renderer.end_frame();
        
        // FPS counter
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