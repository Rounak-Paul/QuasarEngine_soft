#pragma once

#include <cstdint>

struct Vertex {
    float x, y;  // screen-space for now (later could add z, uvs, etc.)
    uint32_t color; // ARGB
};