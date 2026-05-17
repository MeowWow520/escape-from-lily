//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_MOVABLEENTITY_H
#define ESCAPE_FROM_LILY_MOVABLEENTITY_H
#include "TexturedEntity.h"



class MovableEntity : public TexturedEntity {
    protected:
        // 实体的最大速度，默认值为：TBD
        float m_max_speed{};
        // 实体的加速度，默认值为：TBD。// TODO：待实装
        float m_acceleration{};
        // 实体的运动方向
        glm::vec2 m_vector{};
    public:
        MovableEntity();
        ~MovableEntity() override = default;
        // getter 和 setter
        [[nodiscard]] float GetMaxSpeed() const;
        float SetMaxSpeed(float newmaxspeed);
        [[nodiscard]] float GetAcceleration() const;
        float SetAcceleration(float newacceleration);
        [[nodiscard]] glm::vec2 GetVector() const;
        glm::vec2 SetVector(glm::vec2 newvector);
};


#endif //ESCAPE_FROM_LILY_MOVABLEENTITY_H
