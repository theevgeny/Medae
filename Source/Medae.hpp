#pragma once

#include <Graphics/IRender.hpp>
#include <Util/ProgramArgs.hpp>
#include <memory>
#include <thread>

class Medae final {
    static Medae *instance;
    IRender *m_render;
    static std::unique_ptr<std::thread> main_thread;
    Medae &operator=(Medae &) = delete;
    Medae(Medae &) = delete;
    Medae(Medae &&) = delete;
    Medae &operator=(Medae &&) = delete;
    Medae(ProgramArgs args);

  public:
    void emergencyShotDown();
    static Medae& getInstance();
    static void initialize(const ProgramArgs& args);
    std::thread& getMainThread();
};