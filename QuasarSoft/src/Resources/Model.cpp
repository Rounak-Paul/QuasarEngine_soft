#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <iostream>

Model::Model(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string basedir = filename.substr(0, filename.find_last_of("/\\") + 1);

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), basedir.c_str());

    if (!warn.empty()) std::cerr << "WARN: " << warn << "\n";
    if (!err.empty()) std::cerr << "ERR: " << err << "\n";
    if (!ret) {
        std::cerr << "Failed to load/parse .obj\n";
        return;
    }

    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
        verts_.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);

    for (size_t i = 0; i < attrib.normals.size(); i += 3)
        normals_.emplace_back(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]);

    for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
        texcoords_.emplace_back(attrib.texcoords[i], attrib.texcoords[i + 1]);

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            std::vector<int> face;
            for (int v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                face.push_back(idx.vertex_index);
            }
            faces_.push_back(face);
            index_offset += fv;
        }
    }

    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {}

int Model::nverts() const { return (int)verts_.size(); }
int Model::nfaces() const { return (int)faces_.size(); }

Vec3f Model::vert(int i) const { return verts_[i]; }
Vec3f Model::normal(int i) const { return normals_.empty() ? Vec3f() : normals_[i]; }
Vec2f Model::texcoord(int i) const { return texcoords_.empty() ? Vec2f() : texcoords_[i]; }

std::vector<int> Model::face(int idx) const { return faces_[idx]; }