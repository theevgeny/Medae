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
FetchContent_MakeAvailable(glad)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(glfw)

glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE API gl:core=4.6)

add_executable(Medae source/main.cpp)

target_link_libraries(Medae PRIVATE glm glfw glad_gl_core_43)

