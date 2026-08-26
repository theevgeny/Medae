cmake_minimum_required(VERSION 3.10)
project(Medae)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.3
)
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.5.1
)
FetchContent_Declare(
	glad
	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG v2.0.8
	GIT_SHALLOW 1
	SOURCE_SUBDIR cmake
)
FetchContent_Declare(
	EnTT
	GIT_REPOSITORY https://github.com/skypjack/entt.git
	GIT_TAG v4.0.0
)
FetchContent_Declare(
	JoltPhysics
	GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics
	GIT_TAG v5.6.0
	SOURCE_SUBDIR Build
)
FetchContent_Declare(
	openal
	GIT_REPOSITORY https://github.com/kcat/openal-soft.git
	GIT_TAG 1.25.2
)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_PROGRESS TRUE
  GIT_TAG boost-1.82.0
)
FetchContent_Declare(
	tomlplusplus
	GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
	GIT_TAG v3.4.0
)
FetchContent_Declare(
	spdlog
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(glm glfw glad entt JoltPhysics openal Boost tomlplusplus spdlog)

glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE API gl:core=4.6)

add_executable(server
	Source/Server/Main.cpp

	Source/Server/Server.cpp
)

target_link_libraries(server PRIVATE glm glfw glad_gl_core_43 EnTT Jolt openal Boost::asio tomlplusplus spdlog)

