#pragma once

#include <string>

enum class ShaderType { Vertex, Fragment, Geometry, Compute };
std::string shaderTypeToString(ShaderType type);

class Shader {
    std::string rawData;
    ShaderType type;

  public:
    Shader(const std::string &path, ShaderType type);
    Shader(const char *rawData, ShaderType type);

    const std::string &getShaderData() const;
    ShaderType getShaderType() const;
};