#include <Graphics/Renders/OpenGL/OpenGLRender.hpp>
#include <Medae.hpp>
#include <iostream>
#include <ostream>

Medae::Medae(ProgramArgs args) {
    std::cout << args.getFlag("test1") << std::endl;
    std::cout << args.getValue("test2") << std::endl;
    m_render = new OpenGLRender();
    m_render->renderLoop([](IRender *render) {
        render->drawTriangle({1, 1}, {1, 0}, {0, 1}, {0, 1, 1});
        render->drawTriangle({0, 0}, {1, 0}, {0, 1}, {1, 1, 0});
    });
}

Medae::~Medae() { delete m_render; }
