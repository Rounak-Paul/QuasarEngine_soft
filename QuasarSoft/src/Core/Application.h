#pragma once
#include <Renderer/Renderer.h>
#include <cstdint>

class Application {
public:
    Application();
    ~Application();

    bool init();
    void run();
    void shutdown();

private:
    bool running = true;
    Renderer renderer;
    int frameCount = 0;
    uint32_t lastTime = 0;
};