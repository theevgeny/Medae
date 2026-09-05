cmake_minimum_required(VERSION 3.10)
project(Medae)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(SODIUM_DISABLE_TESTS ON)

include(FetchContent)

FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG v2.0.8
	GIT_SHALLOW 1
	SOURCE_SUBDIR cmake
)
FetchContent_Declare(
	JoltPhysics
	GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics
	GIT_TAG v5.6.0
	SOURCE_SUBDIR Build
)

FetchContent_Declare(
  Sodium
  GIT_REPOSITORY https://github.com/robinlinden/libsodium-cmake.git
  GIT_TAG        9b2848dfc1b917a9410f0de9d81059b26cbfaa8d  # 请替换为你需要的具体commit
)

FetchContent_MakeAvailable(glad JoltPhysics Sodium)

find_package(Boost 1.92 REQUIRED COMPONENTS asio program_options filesystem)
find_package(glfw3 3.5 REQUIRED)
find_package(tomlplusplus 3.4 REQUIRED)
find_package(OpenAL 1.25.2 REQUIRED)
find_package(glm 1.0.3 REQUIRED)
find_package(spdlog 1.17.0 REQUIRED)
find_package(EnTT 4.0.0 REQUIRED)
find_package(zstd 1.5.7 REQUIRED)

glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE API gl:core=4.6)

add_executable(server
	Source/Server/Main.cpp

	Source/Server/Server.cpp
	Source/Server/Network.cpp
	Source/Server/PropertiesConfig.cpp
	Source/Server/ArgumentsParser.cpp

	Source/Network/Network.cpp

	Source/Utils/Compression.cpp
)

target_link_libraries(server PRIVATE Boost::asio Boost::filesystem Boost::program_options tomlplusplus spdlog::spdlog zstd::libzstd sodium)

target_include_directories(server PRIVATE Source)

add_executable(client
	Source/Client/Main.cpp
	
	Source/Client/Client.cpp

	Source/Graphics/Graphics.cpp

	Source/Network/Network.cpp
	
	Source/Utils/Compression.cpp
)

target_link_libraries(client PRIVATE glm glfw glad_gl_core_43 EnTT::EnTT Jolt OpenAL::OpenAL Boost::filesystem Boost::asio Boost::program_options spdlog::spdlog zstd::libzstd sodium)

target_include_directories(client PRIVATE Source)

add_link_options(-Wl,--gc-sections,-s)
add_compile_options(
	-fcolor-diagnostics
	-ffunction-sections
	-fdata-sections
	-O2
	-g
)

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:server>
)

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_STRIP} --strip-all $<TARGET_FILE:client>
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

