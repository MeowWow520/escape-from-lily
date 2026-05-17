//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#define ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#include "Input.h"
#include <glm/glm.hpp>


class KeyboardInput : public Input {
    public:
        KeyboardInput();
        bool SetDefaultKeyBind() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;

        glm::vec2 GetMovementNormalizeVec2();



};

#endif //ESCAPE_FROM_LILY_KEYBOARDINPUT_H
