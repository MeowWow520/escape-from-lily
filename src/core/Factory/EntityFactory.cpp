//
// Created by MeowWow520 on 2026/5/20.
//

#include "EntityFactory.h"

#include "../Logger/Log.h"
#include "../Entities/Background.h"
#include "../Entities/Camera.h"
#include "../Entities/Player.h"


EntityPtr<Object> EntityFactory::Create(const EntityType type, const EntityParams &params) const {
    EntityPtr<Object> entity;

    switch (type) {
        case EntityType::Background: {
            if (!std::holds_alternative<BackgroundParams>(params)) {
                EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::Background");
                return nullptr;
            }
            const auto&[texture_path, world_pos] = std::get<BackgroundParams>(params);
            EntityPtr<Background> bg(new Background());
            bg->SetPath(texture_path);
            bg->SetWorldPos(world_pos);
            entity = std::move(bg);
            break;
        }
        case EntityType::Camera: {
            if (!std::holds_alternative<CameraParams>(params)) {
                EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::Camera");
                return nullptr;
            }
            const auto&[world_pos] = std::get<CameraParams>(params);
            EntityPtr<Camera> ca(new Camera());
            ca->SetWorldPos(world_pos);
            entity = std::move(ca);
            break;
        }
        case EntityType::Player: {
            if (!std::holds_alternative<PlayerParams>(params)) {
                EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::Player");
                return nullptr;
            }
            const auto&[name, texture_path, world_pos] = std::get<PlayerParams>(params);
            EntityPtr<Player> pl(new Player());
            pl->SetPath(texture_path);
            pl->SetWorldPos(world_pos);
            pl->SetName(name);
            entity = std::move(pl);
            break;
        }
        case EntityType::UserInterface: {
            break;
        }
            default: {
            EFL_LOGGER_ERROR(LogCategory::Factory, "Unknow entity");
            break;
        }
    }
    if (entity->Initialize() != 0) {
        EFL_LOGGER_ERROR(LogCategory::Factory, "Initialize entity failed");
        return nullptr;
    }
    EFL_LOGGER_INFO(LogCategory::Factory, "Entity initialized: Type = {}", entity->GetName());
    return entity;
}
