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

        static EntityPtr<Camera> createCamera(const EntityParams& params);
        static EntityPtr<Background> createBackground(const EntityParams& params);
        static EntityPtr<Player> createPlayer(const EntityParams& params);
        static EntityPtr<TextStatic> createTextStatic(const EntityParams& params);
};


#endif //ESCAPE_FROM_LILY_ENTITYFACTORY_H
