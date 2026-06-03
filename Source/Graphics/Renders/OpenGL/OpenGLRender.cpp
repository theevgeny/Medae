#include "Graphics/Shader.hpp"
#include "glm/ext/vector_float3.hpp"
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
    Shader vertex{}, fragment{};
    vertex.load("v.glsl", GL_VERTEX_SHADER);
    fragment.load("f.glsl", GL_FRAGMENT_SHADER);
    vertex.use();
    fragment.use();
}
OpenGLRender::OpenGLRender(int windowXSize, int windowYSize, char *windowName) {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, false);
    m_window = glfwCreateWindow(windowXSize, windowYSize, windowName, nullptr,
                                nullptr);
    wight = windowXSize;
    height = windowYSize;
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        exit(1001);
    }
    Shader vertex{}, fragment{};
    vertex.load("v.glsl", GL_VERTEX_SHADER);
    fragment.load("f.glsl", GL_FRAGMENT_SHADER);
    vertex.use();
    fragment.use();
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

void OpenGLRender::rendMesh(Mesh &mesh) {
    static unsigned int VAO = 0, VBO = 0, EBO = 0;

    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVerticesCount() * sizeof(glm::vec3),
                 mesh.getRawVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndicesCount() * sizeof(int),
                 mesh.getRawIndices(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);
    glDrawElements(GL_TRIANGLES, mesh.getIndicesCount(), GL_UNSIGNED_INT,
                   nullptr);
    glBindVertexArray(0);
}
