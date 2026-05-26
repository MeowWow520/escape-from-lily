//
// Created by MeowWow520 on 2026/5/23.
//

#ifndef ESCAPE_FROM_LILY_TEXTBASE_H
#define ESCAPE_FROM_LILY_TEXTBASE_H
#include <string>

#include "TextStypes.h"
#include "../Object/ObjectScreen.h"

class TextBase : public ObjectScreen {
    protected:
        StringParam m_string;
    public:
        explicit TextBase(const char* m_entity_name = "TextBase")
            : ObjectScreen(m_entity_name) { }

        ~TextBase() override = default;
        // setter 和 getter
        std::string SetStringParam_text(std::string text);
        [[nodiscard]] std::string GetStringParam_text() const;
        float SetTextParam_font_size(float size);
        [[nodiscard]] float GetTextParam_font_size() const;
        SDL_Color SetTextParam_color(SDL_Color color);
        [[nodiscard]] SDL_Color GetTextParam_color() const;
        std::string SetTextParam_font_path(std::string path);
        SDL_Color SetTextParam_back_color(SDL_Color color);
        [[nodiscard]] SDL_Color GetTextParam_back_color() const;
        float SetTextParam_rotation(float rotation);
        [[nodiscard]] float GetTextParam_rotation() const;
        glm::vec2 SetTextParam_scale(glm::vec2 scale);
        [[nodiscard]] glm::vec2 GetTextParam_scale() const;
        int SetTextParam_text_display_time(int time);




        bool StepCurrentChar(int step);
        int InitializeDefaultTextParam(TextStype stype);
};


#endif //ESCAPE_FROM_LILY_TEXTBASE_H
