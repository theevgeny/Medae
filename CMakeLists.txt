cmake_minimum_required(VERSION 3.10)
project(Medae)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)

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

FetchContent_MakeAvailable(glad JoltPhysics)

find_package(Boost 1.92 REQUIRED COMPONENTS asio)
find_package(glfw3 3.5 REQUIRED)
find_package(tomlplusplus 3.4 REQUIRED)
find_package(OpenAL 1.25.2 REQUIRED)
find_package(glm 1.0.3 REQUIRED)
find_package(spdlog 1.17.0 REQUIRED)
find_package(EnTT 4.0.0 REQUIRED)

glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE API gl:core=4.6)

add_executable(server
	Source/Server/Main.cpp

	Source/Server/Server.cpp
)

target_link_libraries(server PRIVATE Boost::asio tomlplusplus spdlog::spdlog)

target_include_directories(server PRIVATE Source)

add_executable(client
	Source/Client/Main.cpp
	
	Source/Client/Client.cpp

	Source/Graphics/Graphics.cpp
)

target_link_libraries(client PRIVATE glm glfw glad_gl_core_43 EnTT::EnTT Jolt OpenAL::OpenAL Boost::asio spdlog::spdlog)

target_include_directories(client PRIVATE Source)

add_link_options(-Wl,--gc-sections,-s)
add_compile_options(
	-fcolor-diagnostics
	-ffunction-sections
	-fdata-sections
	-Oz
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

