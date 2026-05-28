//
// Created by MeowWow520 on 2026/5/27.
//

#ifndef ESCAPE_FROM_LILY_TEXTSTATIC_H
#define ESCAPE_FROM_LILY_TEXTSTATIC_H
#include "TextBase.h"


class TextStatic : public TextBase {
    public:
        explicit TextStatic(const char* m_entity_name = "TextStatic")
            : TextBase(m_entity_name) {}
        ~TextStatic() override = default;


        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Render() override;
        void Update(float dt) override;
        int Quit() override;
};



#endif //ESCAPE_FROM_LILY_TEXTSTATIC_H
