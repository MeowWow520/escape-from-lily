//
// Created by MeowWow520 on 2026/5/11.
//

#ifndef ESCAPE_FROM_LILY_BACKGROUND_H
#define ESCAPE_FROM_LILY_BACKGROUND_H
#include "TexturedEntity.h"


class Background : public TexturedEntity {
    protected:
        std::string m_path;
    public:
        int Initialize() override;
        void HandleEvents(SDL_Event event) override;
        void Update(float dt) override;
        void Render() override;
        int Quit() override;
};


#endif //ESCAPE_FROM_LILY_BACKGROUND_H
