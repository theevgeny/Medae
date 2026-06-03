#pragma once

#include <Graphics/IRender.hpp>
#include <Util/ProgramArgs.hpp>


class Medae final {
    IRender *m_render;
    Medae &operator=(Medae &) = delete;
    Medae(Medae &) = delete;
    Medae(Medae &&) = delete;
    Medae &operator=(Medae &&) = delete;

  public:
    Medae(ProgramArgs args);
    ~Medae();
};