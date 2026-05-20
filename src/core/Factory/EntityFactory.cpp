//
// Created by MeowWow520 on 2026/5/20.
//

#include "EntityFactory.h"

#include "../Logger/Log.h"
#include "../Entities/Background.h"


EntityPtr<Object> EntityFactory::Create(EntityType type, const EntityParams &params) const {
    EntityPtr<Object> entity;

    switch (type) {
        case EntityType::Background: {
            if (!std::holds_alternative<BackgroundParams>(params)) {
                EFL_LOGGER_ERROR(LogCategory::Factory, "Wrong params pass to EntityType::Background");
                return nullptr;
            }
            const auto&[texture_path, world_pos] = std::get<BackgroundParams>(params);
            auto bg = std::make_unique<Background>();
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
            const auto& c = std::get<CameraParams>(params);
            auto ca = std::make_unique<Camera>();
        }
    }
}
