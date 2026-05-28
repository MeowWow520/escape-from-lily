//
// Created by MeowWow520 on 2026/5/20.
//

#ifndef ESCAPE_FROM_LILY_ENTITYTYPES_H
#define ESCAPE_FROM_LILY_ENTITYTYPES_H
#include <memory>
#include <string>
#include <variant>
#include <glm/vec2.hpp>

#include "../Font/TextStypes.h"
#include "../Object/Object.h"

enum class EntityType : uint8_t {
    Player,
    Camera,
    Background,
    UserInterface,
    TextStatic
    // upcoming
};

struct PlayerParams {
    std::string m_player_name;
    std::string texture_path;
};

struct CameraParams {
    glm::vec2 world_pos;
    float m_border;
};

struct BackgroundParams {
    std::string texture_path;
    glm::vec2 world_pos;
};

struct UserInterfaceParams {
    std::string texture_path;
    glm::vec2 screen_position;
};
struct TextStaticParams {
    std::string text;
    float font_size;
    SDL_Color color;
    glm::vec2 screen_pos;
    float display_time;
};


using EntityParams = std::variant<
    std::monostate,
    PlayerParams,
    CameraParams,
    BackgroundParams,
    UserInterfaceParams,
    TextStaticParams
>;

struct EntityDeleter {
    void operator()(Object* p) const noexcept {
        if (p) {
            p->Quit();
            delete p;
        }
    }
};

template <typename T = Object>
using EntityPtr = std::unique_ptr<T, EntityDeleter>;



#endif //ESCAPE_FROM_LILY_ENTITYTYPES_H
