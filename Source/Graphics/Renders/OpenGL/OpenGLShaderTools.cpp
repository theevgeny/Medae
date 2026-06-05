#include <Graphics/Renders/OpenGL/OpenGLShaderTools.hpp>
#include <Macros.hpp>
#include <Util/CriticalError.hpp>
#include <functional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <initializer_list>
#include <vector>


unsigned int OpenGLShaderTools::addShaderProgramToOpenGL(
    std::initializer_list<std::reference_wrapper<Shader>> shaders) {
    std::vector<unsigned int> shaderIds;
    for (auto &shaderRef : shaders) {
        Shader shaderSource = shaderRef.get();
        shaderIds.push_back(compileShader(shaderSource));
    }
    unsigned int programId = glCreateProgram();
    for (unsigned int shaderId : shaderIds)
        glAttachShader(programId, shaderId);
    glLinkProgram(programId);
    int status;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if (status != true) {
        GLint logLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
        std::string errorLog;
        if (logLength > 0) {
            std::vector<char> log(logLength);
            glGetProgramInfoLog(programId, logLength, nullptr, log.data());
            errorLog = log.data();
        }
        throwCriticalError(M_PP + "Shader linking failed: " + errorLog);
    }
    for (unsigned int shaderId : shaderIds) {
        glDetachShader(programId, shaderId);
        glDeleteShader(shaderId);
    }
    return programId;
}
void OpenGLShaderTools::useShaderProgramInOpenGL(unsigned int shaderProgramId) {
    glUseProgram(shaderProgramId);
}
void OpenGLShaderTools::deleteShaderProgramInOpenGL(
    unsigned int shaderProgramId) {
    glDeleteProgram(shaderProgramId);
}

void OpenGLShaderTools::deleteShaderInOpenGL(unsigned int shaderId) {
    glDeleteShader(shaderId);
}
unsigned int OpenGLShaderTools::compileShader(const Shader &shader) {
    unsigned int shaderId =
        glCreateShader(shaderTypeToOpenGLShaderType(shader.getShaderType()));
    const char *sourcePtr = shader.getShaderData().c_str();
    glShaderSource(shaderId, 1, &sourcePtr, nullptr);
    glCompileShader(shaderId);
    int status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status != true) {
        int infoLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> infoLog(infoLogLength);
        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, infoLog.data());
        throwCriticalError(M_PP + "Shader compilation failed: " +
                           shaderTypeToString(shader.getShaderType()) +
                           infoLog.data());
    }
    return shaderId;
}
int OpenGLShaderTools::shaderTypeToOpenGLShaderType(ShaderType type) {
    switch (type) {
    case ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderType::Compute:
        return GL_COMPUTE_SHADER;
    case ShaderType::Geometry:
        return GL_GEOMETRY_SHADER;
    }
}