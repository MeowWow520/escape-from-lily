//
// Created by MeowWow520 on 2026/5/14.
//

#ifndef ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#define ESCAPE_FROM_LILY_KEYBOARDINPUT_H
#include "Input.h"



class KeyboardInput : public Input {
    protected:
        bool SetDefaultKeyBind() override;

    public:
        KeyboardInput();
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
};

#endif //ESCAPE_FROM_LILY_KEYBOARDINPUT_H
