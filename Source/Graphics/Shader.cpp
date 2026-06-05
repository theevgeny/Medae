#include <Graphics/Shader.hpp>
#include <Util/CriticalError.hpp>
#include <filesystem>
#include <fstream>
#include <string>

Shader::Shader(const std::string &path, ShaderType type): type(type) {
    if (!std::filesystem::exists(path))
        throwCriticalError("File " + path + " not found");
    std::ifstream file{path, std::ios::binary | std::ios::ate};
    if (!file.is_open())
        throwCriticalError("File " + path + " cannot be opened");
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    rawData = std::string(size, '\0');
    if (!file.read(rawData.data(), size)) throwCriticalError("File " + path + " cannot be read");
    file.close();
}

Shader::Shader(const char *rawData, ShaderType type): type(type) {
    this->rawData = rawData;
}

const std::string& Shader::getShaderData() const {
    return rawData;
}

ShaderType Shader::getShaderType() const {
    return type;
}


std::string shaderTypeToString(ShaderType type) {
    switch (type) {
    case ShaderType::Fragment:
        return "fragment";
    case ShaderType::Vertex:
        return "vertex";
    case ShaderType::Compute:
        return "compute";
    case ShaderType::Geometry:
        return "geometry";
    }
}