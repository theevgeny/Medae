#include <Graphics/Shader.hpp>
#include <fstream>
#include <sstream>

unsigned int Shader::compileShader(const std::string &source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        exit(1005);
    }
    return shader;
}

Shader::Shader() : m_programID(0) {}
Shader::~Shader() {
    if (m_programID)
        glDeleteProgram(m_programID);
}

bool Shader::load(const std::string &shaderPath, int shaderType) {
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile.is_open()) {
        exit(1003);
    }
    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    std::string shaderSource = buffer.str();
    if (shaderSource.empty())
        return false;
    unsigned int shader = compileShader(shaderSource, shaderType);
    if (!shader)
        return false;
    m_programID = glCreateProgram();
    glAttachShader(m_programID, shader);
    glLinkProgram(m_programID);
    glDeleteShader(shader);
    int success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        exit(1004);
    }
    return true;
}

void Shader::use() { glUseProgram(m_programID); }

void Shader::unuse() { glUseProgram(0); }

int Shader::getUniformLocation(const std::string &name) {
    if (m_uniformCache.find(name) != m_uniformCache.end()) {
        return m_uniformCache[name];
    }
    int location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

void Shader::setUniformInt(const std::string &name, int value) {
    glUniform1i(getUniformLocation(name), value);
}
void Shader::setUniformFloat(const std::string &name, float value) {
    glUniform1f(getUniformLocation(name), value);
}
void Shader::setUniformVec3(const std::string &name, const glm::vec3 &value) {
    glUniform3fv(getUniformLocation(name), 1, &value.x);
}
void Shader::setUniformMat4(const std::string &name, const glm::mat4 &value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

unsigned int Shader::getID() const { return m_programID; }