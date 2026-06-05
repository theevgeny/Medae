#pragma once

#include <Graphics/Shader.hpp>

class OpenGLShaderTools final {
  public:
    static unsigned int addShaderProgramToOpenGL(
        std::initializer_list<std::reference_wrapper<Shader>> shaders);
    static void useShaderProgramInOpenGL(unsigned int shaderProgramId);
    static void deleteShaderProgramInOpenGL(unsigned int shaderProgramId);
    static void deleteShaderInOpenGL(unsigned int shaderId);
    static unsigned int compileShader(const Shader &shader);
    static int shaderTypeToOpenGLShaderType(ShaderType type);
};