//
// Created by MeowWow520 on 2026/5/28.
//

#ifndef ESCAPE_FROM_LILY_HEALTHYPOINTENTITY_H
#define ESCAPE_FROM_LILY_HEALTHYPOINTENTITY_H
#include "MovableEntity.h"


class HealthyPointEntity : public MovableEntity {
    protected:
        float m_f_health{};
        float m_f_get_damage{};
    public:
        explicit HealthyPointEntity(const char* m_entity_name = "HealthyPointEntity")
            : MovableEntity(m_entity_name) { }
        ~HealthyPointEntity() override = default;

        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float deltaTime) override;
        void Render() override;
        int Quit() override;
};


#endif //ESCAPE_FROM_LILY_HEALTHYPOINTENTITY_H
