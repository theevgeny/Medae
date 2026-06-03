#include <Graphics/Renders/OpenGL/OpenGLRender.hpp>
#include <cstdlib>


OpenGLRender::OpenGLRender() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, false);
    m_window = glfwCreateWindow(640, 480, "OpenGLRender", nullptr, nullptr);
    wight = 640;
    height = 480;
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(1001);
    }
}
OpenGLRender::OpenGLRender(int windowXSize, int windowYSize, char *windowName) {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, false);
    m_window = glfwCreateWindow(windowXSize, windowYSize, windowName, nullptr,
                                nullptr);
    wight = windowXSize;
    height = windowYSize;
    glfwMakeContextCurrent(m_window);
}
OpenGLRender::~OpenGLRender() { glfwDestroyWindow(m_window); }
void OpenGLRender::setWindowProperties(int windowXSize, int windowYSize,
                                       char *windowName) {
    glfwDestroyWindow(m_window);
    glfwWindowHint(GLFW_RESIZABLE, false);
    m_window = glfwCreateWindow(windowXSize, windowYSize, windowName, nullptr,
                                nullptr);
    wight = windowXSize;
    height = windowYSize;
    glfwMakeContextCurrent(m_window);
};

void OpenGLRender::renderLoop(std::function<void(IRender *)> callback) {
    while (!glfwWindowShouldClose(m_window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        callback(this);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
};

void OpenGLRender::drawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Color c) {
    glBegin(GL_TRIANGLES);
    glColor3d(c.r, c.g, c.b);
    glVertex2d(p1.x, p1.y);
    glVertex2d(p2.x, p2.y);
    glVertex2d(p3.x, p3.y);
    glEnd();
};
