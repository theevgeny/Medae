#include "VulkanRender.hpp"
#include "EngineFactoryVk.h"
#include <X11/Xlib.h>
#include <spdlog/spdlog.h>

using namespace Medae::Graphics;

static const char* VertSource = R"(
    struct PSInput
    {
        float4 Pos   : SV_POSITION;
        float3 Color : COLOR;
    };

    void main(in uint VertId : SV_VertexID,
              out PSInput PSIn)
    {
        float4 Pos[6];
        Pos[0] = float4(-0.5, -0.5, 0.0, 1.0);
        Pos[1] = float4(-0.5, +0.5, 0.0, 1.0);
        Pos[2] = float4(+0.5, +0.5, 0.0, 1.0);
        Pos[3] = float4(-0.5, -0.5, 0.0, 1.0);
        Pos[4] = float4(+0.5, -0.5, 0.0, 1.0);
        Pos[5] = float4(+0.5, +0.5, 0.0, 1.0);

        float3 Col[6];
        Col[0] = float3(1.0, 0.0, 0.0); // red
        Col[1] = float3(0.0, 1.0, 0.0); // green
        Col[2] = float3(0.0, 0.0, 1.0); // blue
        Col[3] = float3(1.0, 0.0, 0.0); // red
        Col[4] = float3(0.0, 1.0, 0.0); // green
        Col[5] = float3(0.0, 0.0, 1.0); // blue

        PSIn.Pos   = Pos[VertId];
        PSIn.Color = Col[VertId];
    }
)";

// Пиксельный шейдер просто интерполирует цвета
static const char* FragSource = R"(
    struct PSInput
    {
        float4 Pos   : SV_POSITION;
        float3 Color : COLOR;
    };

    struct PSOutput
    {
        float4 Color : SV_TARGET;
    };

    void main(in PSInput PSIn,
              out PSOutput PSOut)
    {
        PSOut.Color = float4(PSIn.Color.rgb, 1.0);
    }
)";

VulkanRender::VulkanRender(const std::shared_ptr<WindowFacade>& window)
{
	void** rawPtrs = static_cast<void**>(window->getRaw());
	Display* display = static_cast<Display*>(rawPtrs[0]);
	Window x11Window = static_cast<Window>(reinterpret_cast<uintptr_t>(rawPtrs[1]));

	if (!display || !x11Window) {
		spdlog::critical("Cannot get native descriptors of window");
	}

	m_pFactoryVk = Diligent::GetEngineFactoryVk();
	if (!m_pFactoryVk) {
		spdlog::critical("Cannot load Vulkan-factory Diligent Engine");
	}

	Diligent::EngineVkCreateInfo EngineCI;

#ifdef DEBUG
	EngineCI.EnableValidation = true;
#endif

	m_pFactoryVk->CreateDeviceAndContextsVk(EngineCI, &m_pDevice, &m_pDeviceContext);

	Diligent::SwapChainDesc SCDesc;
	SCDesc.Width = window->getWidth();
	SCDesc.Height = window->getHeight();
	SCDesc.ColorBufferFormat = Diligent::TEX_FORMAT_RGBA8_UNORM_SRGB;
	SCDesc.DepthBufferFormat = Diligent::TEX_FORMAT_D32_FLOAT;

	Diligent::LinuxNativeWindow NativeWindow;
	NativeWindow.pDisplay = display;
	NativeWindow.WindowId = static_cast<Diligent::Uint32>(x11Window);

	m_pFactoryVk->CreateSwapChainVk(m_pDevice, m_pDeviceContext, SCDesc, NativeWindow, &m_pSwapChain);

	m_PSOCreateInfo.PSODesc.Name = "Vulkan PSO";
	m_PSOCreateInfo.PSODesc.PipelineType = Diligent::PIPELINE_TYPE_GRAPHICS;

	m_PSOCreateInfo.GraphicsPipeline.NumRenderTargets = 1;
	m_PSOCreateInfo.GraphicsPipeline.RTVFormats[0] = m_pSwapChain->GetDesc().ColorBufferFormat;
	m_PSOCreateInfo.GraphicsPipeline.DSVFormat = m_pSwapChain->GetDesc().DepthBufferFormat;
	m_PSOCreateInfo.GraphicsPipeline.PrimitiveTopology = Diligent::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

	m_PSOCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = Diligent::CULL_MODE_NONE;
	m_PSOCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = false;

	m_shaderCI.SourceLanguage = Diligent::SHADER_SOURCE_LANGUAGE_HLSL;
	m_shaderCI.Desc.UseCombinedTextureSamplers = true;

	m_shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_VERTEX;
	m_shaderCI.EntryPoint = "main";
	m_shaderCI.Desc.Name = "VERT_SHADER";
	m_shaderCI.Source = VertSource;
	m_pDevice->CreateShader(m_shaderCI, &m_pVS);

	m_shaderCI.Desc.ShaderType = Diligent::SHADER_TYPE_PIXEL;
	m_shaderCI.EntryPoint = "main";
	m_shaderCI.Desc.Name = "FRAG_SHADER";
	m_shaderCI.Source = FragSource;
	m_pDevice->CreateShader(m_shaderCI, &m_pFS);

	m_PSOCreateInfo.pVS = m_pVS;
	m_PSOCreateInfo.pPS = m_pFS;

	m_pDevice->CreateGraphicsPipelineState(m_PSOCreateInfo, &m_pPSO);
}

void VulkanRender::renderLoop(Medae::Graphics::RenderLoopFunction func)
{
	const float ClearColor[] = {0.066f, 0.207f, 0.278f, 1.0f};

	while (true) {
		auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
		auto* pDSV = m_pSwapChain->GetDepthBufferDSV();

		m_pDeviceContext->SetRenderTargets(1, &pRTV, pDSV, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		m_pDeviceContext->ClearRenderTarget(pRTV, ClearColor, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
		m_pDeviceContext->ClearDepthStencil(
			pDSV, Diligent::CLEAR_DEPTH_FLAG, 1.0f, 0, Diligent::RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

		m_pDeviceContext->SetPipelineState(m_pPSO);
		Diligent::DrawAttribs drawAttrs;
		drawAttrs.NumVertices = 6;
		m_pDeviceContext->Draw(drawAttrs);

		m_pSwapChain->Present();
	}
}