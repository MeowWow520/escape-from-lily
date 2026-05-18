//
// Created by MeowWow520 on 2026/5/17.
//

#ifndef ESCAPE_FROM_LILY_PLAYER_H
#define ESCAPE_FROM_LILY_PLAYER_H
#include "Base/MovableEntity.h"


class Player : public MovableEntity {
    protected:
        // 玩家名称
        std::string m_player_name;
    public:
        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
        void Render() override;
        int Quit() override;
};


#endif //ESCAPE_FROM_LILY_PLAYER_H
