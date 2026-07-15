//
// Created by MeowWow520 on 2026/5/20.
//

#ifndef ESCAPE_FROM_LILY_ENTITYFACTORY_H
#define ESCAPE_FROM_LILY_ENTITYFACTORY_H

#include "EntityTypes.h"
#include "../Entities/Player.h"
#include "../Entities/Camera.h"
#include "../Entities/Background.h"
#include "../Font/Base/TextStatic.h"


class EntityFactory {
    public:
        static EntityPtr<Player> createPlayer(const PlayerParams& params);
        static EntityPtr<Camera> createCamera(const CameraParams& params);
        static EntityPtr<Background> createBackground(const BackgroundParams& params);
        static EntityPtr<TextStatic> createTextStatic(const TextStaticParams& params);

    private:
        static SDL_BlendMode stringToBlendMode(const std::string& str);
};


#endif //ESCAPE_FROM_LILY_ENTITYFACTORY_H
