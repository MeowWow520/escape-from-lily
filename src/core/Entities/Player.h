//
// Created by MeowWow520 on 2026/5/17.
//

#ifndef ESCAPE_FROM_LILY_PLAYER_H
#define ESCAPE_FROM_LILY_PLAYER_H
#include "Base/HealthyPointEntity.h"



class Player : public HealthyPointEntity {
    protected:
        // 玩家名称
        std::string m_strPlayerName;
    public:
        explicit Player() : HealthyPointEntity("Player") {}

        int Initialize() override;
        void handleEvents(SDL_Event event) override;
        void Update(float dt) override;
        void Render() override;
        int Quit() override;

        std::string setName(std::string name);
};


#endif //ESCAPE_FROM_LILY_PLAYER_H
