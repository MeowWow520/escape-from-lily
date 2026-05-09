cmake_minimum_required(VERSION 3.28)
project(escape-from-lily)
set(CMAKE_CXX_STANDARD 20)

add_executable(escape-from-lily
        src/main.cpp
        src/core/Game.h
        src/core/Game.cpp
        src/core/Object.h
        src/core/Scene.h
        src/SceneMain.cpp
        src/SceneMain.h
)

find_package(ZLIB REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(escape-from-lily PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
        glm::glm
)