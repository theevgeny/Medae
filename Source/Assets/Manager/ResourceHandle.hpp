#pragma once

#include <cstdint>

namespace Medae::Assets {
struct ResourceHandleBase
{
	uint32_t index = 0, generation = 0;
	bool isValid() const { return index != 0; }
};

template<typename T> struct ResourceHandle : public ResourceHandleBase
{
};

using SoundHandle = ResourceHandle<struct SoundHandleTag>;
using SceneHandle = ResourceHandle<struct SceneHandleTag>;
using TextureHandle = ResourceHandle<struct TextureHandleTag>;
using ModelHandle = ResourceHandle<struct ModelHandleTag>;
using MeshHandle = ResourceHandle<struct MeshHandleTag>;
using MaterialHandle = ResourceHandle<struct MaterialHandleTag>;
using AnimationHandle = ResourceHandle<struct AnimationHandleTag>;
using AnimationControllerHandle = ResourceHandle<struct AnimationControllerHandleTag>;
using RenderControllerHandle = ResourceHandle<struct RenderControllerHandleTag>;
using ScriptHandle = ResourceHandle<struct ScriptHandleTag>;
using ModuleHandle = ResourceHandle<struct ModuleHandleTag>;
using FontHandle = ResourceHandle<struct FontHandleTag>;
using LocalizationHandle = ResourceHandle<struct LocalizationHandleTag>;
using TomlHandle = ResourceHandle<struct TomlHandleTag>;
using RawHandle = ResourceHandle<struct RawHandleTag>;
using TemplateHandle = ResourceHandle<struct TemplateHandleTag>;
} // namespace Medae::Assets