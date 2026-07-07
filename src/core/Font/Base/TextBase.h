//
// Created by MeowWow520 on 2026/5/27.
//

#ifndef ESCAPE_FROM_LILY_TEXTBASE_H
#define ESCAPE_FROM_LILY_TEXTBASE_H
#include "TextStypes.h"
#include "../../Object/ObjectScreen.h"
#include "../FontManager.h"



class TextBase : public ObjectScreen {
    protected:
        FontManager& m_font_manager = FontManager::getInstance();
        BaseString m_text{};
        TTF_Text* m_ttf_text{};
        TTF_Font* m_ttf_font{};
    public:
        explicit TextBase(const char* m_entity_name = "ObjectScreen")
            : ObjectScreen(m_entity_name) {}
        ~TextBase() override = default;

        // setter and getter
        std::string setText(const std::string &new_text) {
            m_text.text = new_text;
            return new_text;
        }
        [[nodiscard]] std::string getText() const { return m_text.text; }
        TextStype setTextStype(const TextStype& new_stype) {
            m_text.text_stype = new_stype;
            return new_stype;
        }
        [[nodiscard]] TextStype getTextStype() const { return m_text.text_stype; }
        float setTextFontSize(const float new_font_size) {
            m_text.font_size = new_font_size;
            return new_font_size;
        }
        [[nodiscard]] float getTextFontSize() const { return m_text.font_size; }
        SDL_Color setTextColor(const SDL_Color new_color) {
            m_text.color = new_color;
            return new_color;
        }
        [[nodiscard]] SDL_Color getTextColor() const { return m_text.color; }
        float setTextRotation(const float new_rotation) {
            m_text.rotation = new_rotation;
            return new_rotation;
        }
        [[nodiscard]] float getTextRotation() const { return m_text.rotation; }
        glm::vec2 setTextPivot(const glm::vec2& new_pivot) {
            m_text.pivot = new_pivot;
            return new_pivot;
        }
        [[nodiscard]] glm::vec2 getTextPivot() const { return m_text.pivot; }
        glm::vec2 setTextScale(const glm::vec2& new_scale) {
            m_text.scale = new_scale;
            return new_scale;
        }
        [[nodiscard]] glm::vec2 getTextScale() const { return m_text.scale; }
        std::string setTextFontPath(const std::string& new_font_path) {
            m_text.font_path = new_font_path;
            return new_font_path;
        }
        [[nodiscard]] std::string getTextFontPath() const { return m_text.font_path; }
        float setTextDisplayTime(const float new_time) {
            m_text.display_time = new_time;
            return new_time;
        }
        [[nodiscard]] float getTextDisplayTime() const { return m_text.display_time; }
        bool setTextIsDisplayTimeOut(const bool new_is_display_time) {
            m_text.is_display_time_out = new_is_display_time;
            return new_is_display_time;
        }
};


#endif //ESCAPE_FROM_LILY_TEXTBASE_H
