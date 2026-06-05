#pragma once

#include <Graphics/IRender.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <unordered_map>

class IRender;
typedef void (*RenderFrameUpdateCallback)(IRender* render);

class OpenGLRender : public IRender {
    GLFWwindow *m_window;
    int wight;
    int height;
    std::unordered_map<std::string, unsigned int> shaderProgramIds;

  public:
    OpenGLRender();
    OpenGLRender(int windowXSize, int windowYSize, char *windowName);
    ~OpenGLRender() override;
    void setWindowProperties(int windowXSize, int windowYSize,
                             char *windowName) override;
    void renderLoop(std::function<void(IRender*)> callback) override;

    void addShaderProgramToPool(
        const std::string &name,
        std::initializer_list<std::reference_wrapper<Shader>> shaders) override;
    void useShaderProgramFromPool(const std::string &name) override;
    void deleteShaderProgramFromPool(const std::string &name) override;

    void rendMesh(Mesh &mesh) override;
};

inline IRender::~IRender() = default;