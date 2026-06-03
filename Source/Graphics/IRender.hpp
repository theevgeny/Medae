#pragma once

#include <Graphics/Color.hpp>
#include <Math/Vector2.hpp>
#include <functional>

class IRender;
typedef void (*RenderFrameUpdateCallback)(IRender* render);

class IRender {
  public:
    virtual void setWindowProperties(int windowXSize, int windowYSize,
                                     char *windowName) = 0;
    virtual void renderLoop(std::function<void(IRender*)> callback) = 0;

    M_DP virtual void drawTriangle(Vector2 p1, Vector2 p2, Vector2 p3, Color c) = 0;

    virtual ~IRender() = 0;
};
