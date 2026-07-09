//
// Created by MeowWow520 on 2026/5/20.
//

#ifndef ESCAPE_FROM_LILY_ENTITYFACTORY_H
#define ESCAPE_FROM_LILY_ENTITYFACTORY_H
#include "EntityTypes.h"
#include "../Entities/Background.h"
#include "../Entities/Camera.h"
#include "../Entities/Player.h"
#include "../Font/Base/TextStatic.h"


class EntityFactory {
    public:
        EntityFactory() = default;
        ~EntityFactory() = default;

        EntityFactory(const EntityFactory&) = default;
        EntityFactory& operator=(const EntityFactory&) = default;

        EntityPtr<Camera> createCamera(const CameraParams& params);
        EntityPtr<Background> createBackground(const BackgroundParams& params);
        EntityPtr<Player> createPlayer(const PlayerParams& params);
        EntityPtr<TextStatic> createTextStatic(const TextStaticParams& params);

    private:
        ConfigManager& m_config_manager = ConfigManager::getInstance();
};


#endif //ESCAPE_FROM_LILY_ENTITYFACTORY_H
