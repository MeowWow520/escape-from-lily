//
// Created by MeowWow520 on 2026/5/17.
//

#ifndef ESCAPE_FROM_LILY_CAMERA_H
#define ESCAPE_FROM_LILY_CAMERA_H
#include "Base/MovableEntity.h"



class Camera : public MovableEntity {
    protected:
        SDL_FRect m_camera_active_range{};
        float m_border{};
    public:
        int Initialize() override;
        void Update(float dt) override;

        // getter 和 setter
        [[nodiscard]] SDL_FRect GetCameraActiveRange() const;
    private:
        [[nodiscard]] bool CanCameraActive() const;
};


#endif //ESCAPE_FROM_LILY_CAMERA_H
