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
        std::cout << args.getFlag("test1") << std::endl;
        std::cout << args.getValue("test2") << std::endl;
        m_render = new OpenGLRender();
        m_render->renderLoop([](IRender *render) {
            render->drawTriangle({1, 1}, {1, 0}, {0, 1}, {0, 1, 1});
            render->drawTriangle({0, 0}, {1, 0}, {0, 1}, {1, 1, 0});
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