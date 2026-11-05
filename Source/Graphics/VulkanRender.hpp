#pragma once

#include <memory>

#include "Common/interface/RefCntAutoPtr.hpp"
#include "DeviceContext.h"
#include "EngineFactoryVk.h"
#include "RenderDevice.h"
#include "SwapChain.h"

#include "Graphics/Window/WindowFacade.hpp"

namespace Medae::Graphics {

class VulkanRender;
using RenderLoopFunction = void(const VulkanRender& render);

class VulkanRender
{
  public:
	VulkanRender(const std::shared_ptr<WindowFacade>& window);
	void renderLoop(RenderLoopFunction func);


  private:
	Diligent::RefCntAutoPtr<Diligent::IRenderDevice> m_pDevice;
	Diligent::RefCntAutoPtr<Diligent::IDeviceContext> m_pDeviceContext;
	Diligent::RefCntAutoPtr<Diligent::ISwapChain> m_pSwapChain;
	Diligent::IEngineFactoryVk* m_pFactoryVk;
	Diligent::GraphicsPipelineStateCreateInfo m_PSOCreateInfo;
	Diligent::ShaderCreateInfo m_shaderCI;
	Diligent::RefCntAutoPtr<Diligent::IPipelineState> m_pPSO;
	Diligent::RefCntAutoPtr<Diligent::IShader> m_pVS;
	Diligent::RefCntAutoPtr<Diligent::IShader> m_pFS;
};
} // namespace Medae::Graphics