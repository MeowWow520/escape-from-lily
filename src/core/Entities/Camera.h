//
// Created by MeowWow520 on 2026/5/17.
//

#ifndef ESCAPE_FROM_LILY_CAMERA_H
#define ESCAPE_FROM_LILY_CAMERA_H
#include "Base/MovableEntity.h"



class Camera : public MovableEntity {
    protected:
        glm::vec2 m_vec2_cameraPos{};
        SDL_FRect m_cameraActiveRange{};
        float m_fBorder{};
    public:
        explicit Camera() : MovableEntity("Camera") {}

        int Initialize() override;
        void Update(float dt) override;

        // setter 和 getter
        [[nodiscard]] SDL_FRect getCameraActiveRange() const;
        float setBorder(float new_border);
        [[nodiscard]] float getBorder() const;
};


#endif //ESCAPE_FROM_LILY_CAMERA_H
