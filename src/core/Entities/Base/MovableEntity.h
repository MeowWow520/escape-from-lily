//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_MOVABLEENTITY_H
#define ESCAPE_FROM_LILY_MOVABLEENTITY_H
#include "TexturedEntity.h"



class MovableEntity : public TexturedEntity {
    protected:
        // 实体的最大速度
        float m_fMaxSpeed{};
        // 实体的加速度，默认值为：TBD。// TODO：待实装
        float m_fAcceleration{};
        // 实体的运动方向
        glm::vec2 m_vec2_vector{};
    public:
        explicit MovableEntity(const char* m_entity_name = "MovableEntity")
            : TexturedEntity(m_entity_name) { }

        ~MovableEntity() override = default;

        int Initialize() override;
        // getter 和 setter
        [[nodiscard]] float getMaxSpeed() const;
        float setMaxSpeed(float new_max_speed);
        [[nodiscard]] float getAcceleration() const;
        float setAcceleration(float new_acceleration);
        [[nodiscard]] glm::vec2 getVector() const;
        glm::vec2 setVector(glm::vec2 new_vector);
};


#endif //ESCAPE_FROM_LILY_MOVABLEENTITY_H
