#include <Macros.hpp>
#include <Graphics/Renders/OpenGL/OpenGLShaderTools.hpp>
#include <Util/CriticalError.hpp>
#include <glm/ext/vector_float3.hpp>
#include <Graphics/Renders/OpenGL/OpenGLRender.hpp>

OpenGLRender::OpenGLRender() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, false);
    m_window = glfwCreateWindow(640, 480, "OpenGLRender", nullptr, nullptr);
    wight = 640;
    height = 480;
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throwCriticalError(M_PP + "GLAD not loaded");
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throwCriticalError(M_PP + "GLAD not loaded");
    }
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

void OpenGLRender::addShaderProgramToPool(
    const std::string &name,
    std::initializer_list<std::reference_wrapper<Shader>> shaders) {
    auto result = shaderProgramIds.emplace(
        name, OpenGLShaderTools::addShaderProgramToOpenGL(shaders));
    if (!result.second)
        throwCriticalError(M_PP + "Attempt to create a shader in pool with the "
                                  "same name as the previous one");
}
void OpenGLRender::useShaderProgramFromPool(const std::string &name) {
    auto it = shaderProgramIds.find(name);
    if (it != shaderProgramIds.end()) {
        glUseProgram(it->second);
    } else
        throwCriticalError(M_PP + "Shader not found in pool");
}
void OpenGLRender::deleteShaderProgramFromPool(const std::string &name) {
    auto it = shaderProgramIds.find(name);
    if (it != shaderProgramIds.end()) {
        int currentProgramId;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramId);
        if ((int)it->second == currentProgramId)
            throwCriticalError(M_PP + "Cannot delete used shader");
        glDeleteProgram(it->second);
    } else
        throwCriticalError(M_PP + "Shader not found in pool");
}

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
