#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh final {
    std::vector<glm::vec3> m_vertices;
    std::vector<int> m_indices;

  public:
    void loadOBJ(const std::string &path);

    Mesh &pushVertex(const glm::vec3 &vertex);
    Mesh &pushIndex(int index);

    float *getRawVertices();
    int getVerticesCount();
    int *getRawIndices();
    int getIndicesCount();
};