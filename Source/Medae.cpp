#include "Graphics/IRender.hpp"
#include "Graphics/Shader.hpp"
#include <Graphics/Renders/OpenGL/OpenGLRender.hpp>
#include <Medae.hpp>
#include <iostream>
#include <memory>
#include <ostream>
#include <thread>

Medae* Medae::instance = nullptr;
std::unique_ptr<std::thread> Medae::main_thread = nullptr;

std::thread &Medae::getMainThread() { return *main_thread; }

Medae::Medae(ProgramArgs args) {
    main_thread = std::make_unique<std::thread>([this, args = std::move(args)]() {
        Mesh testMesh{};
        testMesh.loadOBJ("test.obj");
        Shader vshader("v.glsl", ShaderType::Vertex);
        Shader fshader("f.glsl", ShaderType::Fragment);
        m_render = std::make_unique<OpenGLRender>();
        m_render->addShaderProgramToPool("test", {vshader, fshader});
        m_render->useShaderProgramFromPool("test");
        m_render->renderLoop([&testMesh](IRender *render) {
            render->rendMesh(testMesh);
        });
    });
}

void Medae::initialize(const ProgramArgs &args) {
    if (!instance) {
        instance = new Medae(args);
    }
}
Medae &Medae::getInstance() { return *instance; }

void Medae::emergencyShotDown() {
}