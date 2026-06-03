#include <Graphics/Mesh.hpp>
#include <cstdlib>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void Mesh::loadOBJ(const std::string &path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                path.c_str());
    if (!warn.empty() || !err.empty() || !ret) {
        exit(1002);
    }
    m_vertices.clear();
    m_indices.clear();
    for (size_t i = 0; i < attrib.vertices.size() / 3; i++) {
        double x = attrib.vertices[3 * i + 0];
        double y = attrib.vertices[3 * i + 1];
        double z = attrib.vertices[3 * i + 2];
        m_vertices.push_back(glm::vec3(x, y, z));
    }
    for (tinyobj::shape_t &shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];
            if (fv == 3) {
                for (int v = 0; v < 3; v++) {
                    tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                    m_indices.push_back(idx.vertex_index);
                }
            }
            index_offset += fv;
        }
    }
}

Mesh &Mesh::pushVertex(const glm::vec3 &vertex) {
    m_vertices.push_back(vertex);
    return *this;
}
Mesh &Mesh::pushIndex(int index) {
    m_indices.push_back(index);
    return *this;
}

double *Mesh::getRawVertices() { return (double *)m_vertices.data(); }
int Mesh::getVerticesCount() { return m_vertices.size(); }
int *Mesh::getRawIndices() { return m_indices.data(); }
int Mesh::getIndicesCount() { return m_indices.size(); }