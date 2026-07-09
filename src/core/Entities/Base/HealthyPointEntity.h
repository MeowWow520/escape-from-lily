//
// Created by MeowWow520 on 2026/5/28.
//

#ifndef ESCAPE_FROM_LILY_HEALTHYPOINTENTITY_H
#define ESCAPE_FROM_LILY_HEALTHYPOINTENTITY_H
#include "MovableEntity.h"


class HealthyPointEntity : public MovableEntity {
    protected:
        float m_fHealth{-1.0f};
        float m_fGetDamage{0.0f};
    public:
        explicit HealthyPointEntity(const char* m_entity_name = "HealthyPointEntity")
            : MovableEntity(m_entity_name) { }
        ~HealthyPointEntity() override = default;

        int Initialize() override;
        void handleEvents(SDL_Event event) override;
        void Update(float deltaTime) override;
        void Render() override;
        int Quit() override;

        [[nodiscard]] float getHealth() const;
        float setHealth(float new_health);
        [[nodiscard]] float getGetDamage() const;
        float setGetDamage(float new_getDamage);
};


#endif //ESCAPE_FROM_LILY_HEALTHYPOINTENTITY_H
