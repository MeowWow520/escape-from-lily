cmake_minimum_required(VERSION 3.28)
project(escape-from-lily)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(MSVC)
    add_compile_options("/utf-8")
endif()


add_executable(escape-from-lily
        src/main.cpp
        src/SceneMain.cpp
        src/SceneMain.h
        src/core/Def.h
        src/core/Game.cpp
        src/core/Game.h
        src/core/Object.h
        src/core/ObjectScreen.h
        src/core/ObjectWorld.h
        src/core/Scene.h
        src/core/Entities/Base/TexturedEntity.h
        src/core/Entities/Base/TexturedEntity.cpp
        src/core/Entities/Background.cpp
        src/core/Entities/Background.h
        src/core/Input/KeyboardInput.cpp
        src/core/Input/KeyboardInput.h
        src/core/Input/Input.cpp
        src/core/Input/Input.h
        src/core/Entities/Base/MovableEntity.cpp
        src/core/Entities/Base/MovableEntity.h
        src/core/Entities/UserInterface.cpp
        src/core/Entities/UserInterface.h
        src/core/Input/Action.h
)

set(ASSETS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

add_custom_command(
        TARGET escape-from-lily
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${ASSETS_SOURCE_DIR}"
        "$<TARGET_FILE_DIR:escape-from-lily>/assets"
        COMMENT "Copying assets folder to output directory..."
        VERBATIM
)

find_package(ZLIB REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(escape-from-lily PRIVATE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        SDL3_ttf::SDL3_ttf
        glm::glm
        fmt::fmt
)