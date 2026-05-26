//
// Created by MeowWow520 on 2026/5/23.
//

#ifndef ESCAPE_FROM_LILY_FONTMANAGER_H
#define ESCAPE_FROM_LILY_FONTMANAGER_H
#include "TextBase.h"
#include "SDL3_ttf/SDL_textengine.h"
#include "../Game.h"
#include "../Factory/EntityFactory.h"


class FontManager {
    public:
        static FontManager& GetInstance() {
            static FontManager instance;
            return instance;
        }
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;


        int Initialize();
        void HandleEvents(SDL_Event event);
        void Update(float dt);
        void Render();
        int Quit();

        [[nodiscard]] TTF_TextEngine* GetTTFEngine() const;

    private:
        FontManager() = default;
        Game& m_game_instance = Game::GetInstance();
        TTF_TextEngine* m_text_engine{};
        std::unordered_map<std::string, TextBase> m_text_uo_map{};
};


#endif //ESCAPE_FROM_LILY_FONTMANAGER_H