#pragma once

#include <Graphics/Mesh.hpp>
#include <Graphics/Shader.hpp>
#include <functional>
#include <initializer_list>
#include <string>

class IRender;
typedef void (*RenderFrameUpdateCallback)(IRender *render);

class IRender {
  public:
    // Base functions
    virtual void setWindowProperties(int windowXSize, int windowYSize,
                                     char *windowName) = 0;
    virtual void renderLoop(std::function<void(IRender *)> callback) = 0;

    // Shader functions
    virtual void addShaderProgramToPool(
        const std::string &name,
        std::initializer_list<std::reference_wrapper<Shader>> shaders) = 0;
    virtual void useShaderProgramFromPool(const std::string &name) = 0;
    virtual void deleteShaderProgramFromPool(const std::string &name) = 0;

    // Rend functions
    virtual void rendMesh(Mesh &mesh) = 0;

    virtual ~IRender() = 0;
};
