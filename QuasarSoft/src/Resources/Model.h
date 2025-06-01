#pragma once
#include <string>
#include <vector>
#include "Geometry.h" // Your Vec3f and Vec2f

class Model {
public:
    Model(const std::string& filename);
    ~Model();

    int nverts() const;
    int nfaces() const;

    Vec3f vert(int i) const;
    Vec3f normal(int i) const;
    Vec2f texcoord(int i) const;
    std::vector<int> face(int idx) const;

private:
    std::vector<Vec3f> verts_;
    std::vector<Vec3f> normals_;
    std::vector<Vec2f> texcoords_;
    std::vector<std::vector<int>> faces_;
};