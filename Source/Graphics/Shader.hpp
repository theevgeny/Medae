#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
private:
    unsigned int m_programID;
    std::unordered_map<std::string, int> m_uniformCache;
    
    int getUniformLocation(const std::string& name);
    unsigned int compileShader(const std::string& source, GLenum type);
    
public:
    Shader();
    ~Shader();
    
    bool load(const std::string& path, int shaderType);
    void use();
    void unuse();
    
    void setUniformInt(const std::string& name, int value);
    void setUniformFloat(const std::string& name, float value);
    void setUniformVec3(const std::string& name, const glm::vec3& value);
    void setUniformMat4(const std::string& name, const glm::mat4& value);
    
    unsigned int getID() const;
};