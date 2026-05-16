//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#define ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#include "Input.h"



class KeyboardInput : public Input {
    public:
        KeyboardInput();
        bool SetDefaultKeyBind() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
};

#endif //ESCAPE_FROM_LILY_KEYBOARDINPUT_H
