cmake_minimum_required(VERSION 3.10)
project(Medae)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 13)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include cstdint")
endif()

set(FETCHCONTENT_QUIET OFF CACHE BOOL "" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(SODIUM_DISABLE_TESTS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

option(USE_OPENGL "Enable Diligent OpenGL backend" ON)
option(USE_VULKAN "Enable Diligent Vulkan backend" ON)

if(WIN32)
    option(USE_D3D11 "Enable Diligent Direct3D11 backend" ON)
    option(USE_D3D12 "Enable Diligent Direct3D12 backend" ON)
else()
    set(USE_D3D11 OFF)
    set(USE_D3D12 OFF)
endif()

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_Declare(
	JoltPhysics
	GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics
	GIT_TAG v5.6.0
	GIT_SHALLOW 1
	SOURCE_SUBDIR Build
)
FetchContent_Declare(
    DiligentCore
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentCore.git
    GIT_TAG v2.5.6
    SOURCE_DIR _deps/DiligentCore
)
FetchContent_Declare(
    DiligentTools
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentTools.git
    GIT_TAG v2.5.6
    SOURCE_DIR _deps/DiligentTools
)
FetchContent_Declare(
    DiligentFX
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentFX.git
    GIT_TAG v2.5.6
    SOURCE_DIR _deps/DiligentFX
)
FetchContent_Declare(
  Sodium
  GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
  GIT_TAG        9b2848dfc1b917a9410f0de9d81059b26cbfaa8d
  GIT_SHALLOW 1
)

if(USE_OPENGL)
    set(DILIGENT_NO_OPENGL OFF CACHE BOOL "" FORCE)
else()
    set(DILIGENT_NO_OPENGL ON CACHE BOOL "" FORCE)
endif()

if(USE_VULKAN)
    set(DILIGENT_NO_VULKAN OFF CACHE BOOL "" FORCE)
    set(DILIGENT_NO_GLSLANG OFF CACHE BOOL "" FORCE)
else()
    set(DILIGENT_NO_VULKAN ON CACHE BOOL "" FORCE)
    set(DILIGENT_NO_GLSLANG ON CACHE BOOL "" FORCE)
endif()

if(WIN32 AND USE_D3D11)
    set(DILIGENT_NO_DIRECT3D11 OFF CACHE BOOL "" FORCE)
else()
    set(DILIGENT_NO_DIRECT3D11 ON CACHE BOOL "" FORCE)
endif()

if(WIN32 AND USE_D3D12)
    set(DILIGENT_NO_DIRECT3D12 OFF CACHE BOOL "" FORCE)
else()
    set(DILIGENT_NO_DIRECT3D12 ON CACHE BOOL "" FORCE)
endif()

set(DILIGENT_NO_METAL        ON CACHE BOOL "" FORCE)
set(DILIGENT_BUILD_SAMPLES   OFF CACHE BOOL "" FORCE)
set(DILIGENT_BUILD_DEMOS     OFF CACHE BOOL "" FORCE)
set(DILIGENT_BUILD_FX        OFF CACHE BOOL "" FORCE)
set(DILIGENT_BUILD_UNITY_PLUGIN OFF CACHE BOOL "" FORCE)
set(DILIGENT_BUILD_RENDER_SCRIPT OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(JoltPhysics Sodium stb DiligentCore DiligentTools DiligentFX)

add_library(stb::stb INTERFACE IMPORTED)
target_include_directories(stb::stb INTERFACE ${stb_SOURCE_DIR})

find_package(Boost 1.92 REQUIRED COMPONENTS asio program_options headers filesystem)
find_package(tomlplusplus 3.4 REQUIRED)
find_package(OpenAL 1.25.2 REQUIRED)
find_package(spdlog 1.17.0 REQUIRED)
find_package(EnTT 4.0.0 REQUIRED)
find_package(zstd 1.5.7 REQUIRED)

set(DILIGENT_RENDERER_LIBS "")
if(USE_OPENGL)
    list(APPEND DILIGENT_RENDERER_LIBS Diligent-GraphicsEngineOpenGL-shared)
endif()
if(USE_VULKAN)
    list(APPEND DILIGENT_RENDERER_LIBS Diligent-GraphicsEngineVk-shared)
endif()
if(WIN32)
    if(USE_D3D11)
        list(APPEND DILIGENT_RENDERER_LIBS Diligent-GraphicsEngineD3D11-shared)
    endif()
    if(USE_D3D12)
        list(APPEND DILIGENT_RENDERER_LIBS Diligent-GraphicsEngineD3D12-shared)
    endif()
endif()

set(MEDAE_RENDERER_DEFINES "")
if(USE_OPENGL)
    list(APPEND MEDAE_RENDERER_DEFINES USE_OPENGL=1)
endif()
if(USE_VULKAN)
    list(APPEND MEDAE_RENDERER_DEFINES USE_VULKAN=1)
endif()
if(WIN32 AND USE_D3D11)
    list(APPEND MEDAE_RENDERER_DEFINES USE_D3D11=1)
endif()
if(WIN32 AND USE_D3D12)
    list(APPEND MEDAE_RENDERER_DEFINES USE_D3D12=1)
endif()

add_executable(server
	Source/Server/Main.cpp

	Source/Server/Server.cpp
	Source/Server/Network.cpp
	Source/Server/PropertiesConfig.cpp
	Source/Server/ArgumentsParser.cpp

	Source/Network/Network.cpp

	Source/Utils/Compression.cpp
)
add_executable(client
	Source/Client/Main.cpp

	Source/Graphics/Window/LinuxWindow.cpp
	Source/Graphics/Window/WindowFactory.cpp
	Source/Graphics/VulkanRender.cpp
)

target_link_libraries(server PRIVATE stb::stb Boost::headers
    Boost::asio Boost::filesystem Boost::program_options
    tomlplusplus spdlog::spdlog zstd::libzstd sodium)
target_link_libraries(client PRIVATE
	${DILIGENT_RENDERER_LIBS} Diligent-TextureLoader
    stb::stb EnTT::EnTT Jolt OpenAL::OpenAL Boost::headers
    Boost::filesystem Boost::asio Boost::program_options
    spdlog::spdlog zstd::libzstd sodium X11 Xext)

target_compile_definitions(client PRIVATE ${MEDAE_RENDERER_DEFINES})

target_include_directories(client PRIVATE Source     
    ${diligentcore_SOURCE_DIR}
    ${diligenttools_SOURCE_DIR}
    ${diligentfx_SOURCE_DIR})
target_include_directories(server PRIVATE Source)

add_link_options(
	-Wl
	--gc-sections
	-s
	-fsanitize=address
)
add_compile_options(
	-fcolor-diagnostics
	-ffunction-sections
	-fdata-sections
    -fno-exceptions
	-O2
	-g
	-fsanitize=address
	-fno-omit-frame-pointer
)

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:server>
)

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:client>
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
