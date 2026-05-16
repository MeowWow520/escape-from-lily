//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_MOVABLEENTITY_H
#define ESCAPE_FROM_LILY_MOVABLEENTITY_H
#include "TexturedEntity.h"



class MovableEntity : public TexturedEntity {
    protected:
        float m_max_speed{};
        // FIXME: 使用加速度有意义吗？
        float m_acceleration{};
        glm::vec2 m_velocity{};
    public:
        MovableEntity();
        ~MovableEntity() override = default;
        // getter 和 setter
        [[nodiscard]] float GetMaxSpeed() const;
        float SetMaxSpeed(float newmaxspeed);

};


#endif //ESCAPE_FROM_LILY_MOVABLEENTITY_H
