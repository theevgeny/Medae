#include "Util/CriticalError.hpp"
#include <Graphics/IRender.hpp>
#include <Graphics/Renders/OpenGL/OpenGLRender.hpp>
#include <Graphics/Shader.hpp>
#include <Medae.hpp>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <thread>

Medae *Medae::instance = nullptr;
std::unique_ptr<std::thread> Medae::main_thread = nullptr;

std::thread &Medae::getMainThread() { return *main_thread; }

Medae::Medae(ProgramArgs args) {
    main_thread = std::make_unique<std::thread>([this,
                                                 args = std::move(args)]() {
        try {
            Mesh testMesh{};
            testMesh.loadOBJ("test.obj");
            Shader vshader((std::string) "v.glsl", ShaderType::Vertex);
            Shader fshader((std::string) "f.glsl", ShaderType::Fragment);
            m_render = std::make_unique<OpenGLRender>();
            m_render->addShaderProgramToPool("base", {vshader, fshader});
            m_render->useShaderProgramFromPool("base");
            glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
                                         glm::vec3(0.0f, 0.0f, 1.0f),
                                         glm::vec3(0.0f, 1.0f, 0.0f));
            float aspect = 4.0f / 3.0f;
            glm::mat4 projection =
                glm::perspective(glm::radians(80.0f), aspect, 0.001f, 100.0f);
            OpenGLRender &glRender = *((OpenGLRender *)m_render.get());
            glRender.setUniform("base", "u_view", view);
            glRender.setUniform("base", "u_projection", projection);
            m_render->renderLoop([&testMesh, &glRender](IRender *render) {
                try {
                    static float angle = 0.0f;
                    angle += 0.0001f;

                    glm::mat4 animatedModel = glm::mat4(1.0f);
                    animatedModel = glm::translate(animatedModel,
                                                   glm::vec3(0.0f, 0.0f, 5.0f));
                    animatedModel = glm::rotate(animatedModel, angle,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
                    glRender.setUniform("base", "u_model", animatedModel);
                    glRender.setUniform("base", "u_color",
                                        glm::vec3(1.0f, 0.5f, 0.2f));
                    render->rendMesh(testMesh);

                    animatedModel = glm::mat4(1.0f);
                    animatedModel = glm::translate(animatedModel,
                                                   glm::vec3(3.0f, 0.0f, 5.0f));
                    animatedModel = glm::rotate(animatedModel, angle,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
                    glRender.setUniform("base", "u_model", animatedModel);
                    glRender.setUniform("base", "u_color",
                                        glm::vec3(0.5f, 0.2f, 1.0f));
                    render->rendMesh(testMesh);

                    animatedModel = glm::mat4(1.0f);
                    animatedModel = glm::translate(
                        animatedModel, glm::vec3(-3.0f, 0.0f, 5.0f));
                    animatedModel = glm::rotate(animatedModel, angle,
                                                glm::vec3(0.0f, 1.0f, 0.0f));
                    glRender.setUniform("base", "u_model", animatedModel);
                    glRender.setUniform("base", "u_color",
                                        glm::vec3(0.2f, 1.0f, 0.5f));
                    render->rendMesh(testMesh);
                } catch (std::exception e) {
                    throwCriticalError(e.what());
                }
            });
        } catch (std::exception e) {
            throwCriticalError(e.what());
        }
    });
}

void Medae::initialize(const ProgramArgs &args) {
    if (!instance) {
        instance = new Medae(args);
    }
}
Medae &Medae::getInstance() { return *instance; }

void Medae::emergencyShotDown() {}