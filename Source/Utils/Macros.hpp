#pragma once

#define STRINGIZE(x) #x
#define STRINGIZE_LINE(x) STRINGIZE(x)
#define FILEPOINT "(" __FILE__ ":" STRINGIZE_LINE(__LINE__) ")"
#define NODIS [[nodiscard]]
#define DEPR [[deprecated]]
#define NODISM(m) [[nodiscard(m)]]
#define DEPRM(m) [[deprecated(m)]]

#define USE_VULKAN 1

enum PLATFORM_ENUM : unsigned char
{
	LINUX = 0x01,
	WIN = 0x02,
	ANDROID = 0x03,
	MACOS = 0x04,
	IOS = 0x05,
	WEB = 0x06
};

#ifdef __EMSCRIPTEN__
#define PLATFORM_DEC 0x06
#elifdef _WIN32
#define PLATFORM_DEC 0x02
#elifdef _WIN64
#define PLATFORM_DEC 0x02
#elifdef __ANDROID__
#define PLATFORM_DEC 0x03
#elifdef __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define PLATFORM_DEC 0x05
#else
#define PLATFORM_DEC 0x04
#endif
#elifdef __linux__
#define PLATFORM_DEC 0x01
#else
#error "Unknown platform"
#endif

#define PLATFORM PLATFORM_ENUM(PLATFORM_DEC)

enum RENDER_API_FLAGS_ENUM : unsigned char
{
	OPENGL = 0x01,
	VULKAN = 0x02,
	D3D11 = 0x04,
	D3D12 = 0x08,
	METAL = 0x10,
	GLES = 0x20,
	WEBGPU = 0x40,
	WEBGL = 0x80
};

constexpr const bool SUPPORT_WASM_JIT =
#ifdef USE_WASM_JIT
	true
#else
	false
#endif
	;

constexpr const unsigned char RENDER_API_FLAGS = 0
#if USE_D3D11
	+ RENDER_API_FLAGS_ENUM::D3D11
#endif
#if USE_D3D12
	+ RENDER_API_FLAGS_ENUM::D3D12
#endif
#if USE_OPENGL
	+ RENDER_API_FLAGS_ENUM::OPENGL
#endif
#if USE_VULKAN
	+ RENDER_API_FLAGS_ENUM::VULKAN
#endif
#if USE_GLES
	+ RENDER_API_FLAGS_ENUM::GLES
#endif
#if USE_WEBGPU
	+ RENDER_API_FLAGS_ENUM::WEBGPU
#endif
#if USE_WEBGL
	+ RENDER_API_FLAGS_ENUM::WEBGL
#endif
#if USE_METAL
	+ RENDER_API_FLAGS_ENUM::METAL
#endif
	;

#define RENDER_API_COUNT                                                                                               \
	(USE_D3D11 + USE_D3D12 + USE_VULKAN + USE_OPENGL + USE_GLES + USE_METAL + USE_WEBGL + USE_WEBGPU)
#if RENDER_API_COUNT == 0
#error                                                                                                                 \
	"No render API selected. Define one or more of: USE_D3D11, USE_D3D12, USE_VULKAN, USE_OPENGL, USE_GLES, USE_METAL, USE_WEBGL, USE_WEBGPU; = 1"
#endif
