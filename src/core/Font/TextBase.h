//
// Created by MeowWow520 on 2026/5/27.
//

#ifndef ESCAPE_FROM_LILY_TEXTBASE_H
#define ESCAPE_FROM_LILY_TEXTBASE_H
#include "TextStypes.h"
#include "../Object/ObjectScreen.h"
#include "FontManager.h"



class TextBase : public ObjectScreen {
    protected:
        FontManager& m_font_manager = FontManager::GetInstance();
        BaseString m_text{};
        TTF_Text* m_ttf_text{};
        TTF_Font* m_ttf_font{};
    public:
        explicit TextBase(const char* m_entity_name = "ObjectScreen")
            : ObjectScreen(m_entity_name) {}
        ~TextBase() override = default;

        // setter and getter
        std::string SetText(const std::string &new_text) {
            m_text.text = new_text;
            return new_text;
        }
        [[nodiscard]] std::string GetText() const { return m_text.text; }
        TextStype SetText_stype(const TextStype& new_stype) {
            m_text.text_stype = new_stype;
            return new_stype;
        }
        [[nodiscard]] TextStype GetText_stype() const { return m_text.text_stype; }
        float SetText_font_size(const float new_font_size) {
            m_text.font_size = new_font_size;
            return new_font_size;
        }
        [[nodiscard]] float GetText_font_size() const { return m_text.font_size; }
        SDL_Color SetText_color(SDL_Color new_color) {
            m_text.color = new_color;
            return new_color;
        }
        [[nodiscard]] SDL_Color GetText_color() const { return m_text.color; }
        float SetText_rotation(const float new_rotation) {
            m_text.rotation = new_rotation;
            return new_rotation;
        }
        [[nodiscard]] float GetText_rotation() const { return m_text.rotation; }
        glm::vec2 SetText_pivot(const glm::vec2& new_pivot) {
            m_text.pivot = new_pivot;
            return new_pivot;
        }
        [[nodiscard]] glm::vec2 GetText_pivot() const { return m_text.pivot; }
        glm::vec2 SetText_scale(const glm::vec2& new_scale) {
            m_text.scale = new_scale;
            return new_scale;
        }
        [[nodiscard]] glm::vec2 GetText_scale() const { return m_text.scale; }
        std::string SetText_font_path(const std::string& new_font_path) {
            m_text.font_path = new_font_path;
            return new_font_path;
        }
        [[nodiscard]] std::string GetText_font_path() const { return m_text.font_path; }
        float SetText_display_time(const float new_time) {
            m_text.display_time = new_time;
            return new_time;
        }
        [[nodiscard]] float GetText_display_time() const { return m_text.display_time; }
        bool SetText_is_display_time_out(const bool new_is_display_time) {
            m_text.is_display_time_out = new_is_display_time;
            return new_is_display_time;
        }
};


#endif //ESCAPE_FROM_LILY_TEXTBASE_H
