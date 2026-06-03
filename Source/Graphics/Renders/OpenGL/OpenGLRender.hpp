#pragma once

#include <Graphics/IRender.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>

class IRender;
typedef void (*RenderFrameUpdateCallback)(IRender* render);

class OpenGLRender : public IRender {
    GLFWwindow *m_window;
    int wight;
    int height;

  public:
    OpenGLRender();
    OpenGLRender(int windowXSize, int windowYSize, char *windowName);
    ~OpenGLRender() override;
    void setWindowProperties(int windowXSize, int windowYSize,
                             char *windowName) override;
    void renderLoop(std::function<void(IRender*)> callback) override;

    void drawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Color c) override;

    void rendMesh(Mesh& mesh) override;
};

inline IRender::~IRender() = default;