//
// Created by MeowWow520 on 2026/5/24.
//

#include "TextBase.h"

#include "../Def.h"
#include "../Logger/Log.h"


std::string TextBase::SetStringParam_text(std::string text) {
    m_string.text = text;
    return text;
}

std::string TextBase::GetStringParam_text() const {
    return m_string.text;
}

float TextBase::SetTextParam_font_size(const float size) {
    m_string.text_param.font_size = size;
    return size;
}

float TextBase::GetTextParam_font_size() const {
    return m_string.text_param.font_size;
}

SDL_Color TextBase::SetTextParam_color(const SDL_Color color) {
    m_string.text_param.color = color;
    return color;
}

SDL_Color TextBase::GetTextParam_color() const {
    return m_string.text_param.color;
}

std::string TextBase::SetTextParam_font_path(std::string path) {
    m_string.text_param.font_path = path;
    return path;
}

SDL_Color TextBase::SetTextParam_back_color(const SDL_Color color) {
    m_string.text_param.back_color = color;
    return color;
}

SDL_Color TextBase::GetTextParam_back_color() const {
    return m_string.text_param.back_color;
}

float TextBase::SetTextParam_rotation(const float rotation) {
    m_string.text_param.rotation = rotation;
    return rotation;
}

float TextBase::GetTextParam_rotation() const {
    return m_string.text_param.rotation;
}

glm::vec2 TextBase::SetTextParam_scale(const glm::vec2 scale) {
    m_string.text_param.scale = scale;
    return scale;
}

glm::vec2 TextBase::GetTextParam_scale() const {
    return m_string.text_param.scale;
}

int TextBase::SetTextParam_text_display_time(const int time) {
    m_string.text_param.text_display_time = time;
    return time;
}

int TextBase::InitializeDefaultTextParam(const TextStype stype) {
    switch (stype) {
        case TextStype::Static: {
            m_string.is_per_char = false;
            m_string.text_param.text_stype = stype;
            m_string.text_param.font_size = DEFAULT_FONT_SIZE;
            m_string.text_param.text_width = m_string.text.size();
            m_string.text_param.text_each_line_width = DEFAULT_TEXT_EACH_LINE_WIDTH;
            m_string.text_param.color = {DEFAULT_TEXT_COLOR};
            m_string.text_param.line_spacing = DEFAULT_TEXT_LINE_SPACING;
            m_string.text_param.font_path = DEFAULT_FONT_PATH;
            m_string.text_param.display_backgrd = false;
            m_string.text_param.rotation = 0;
            m_string.text_param.scale = glm::vec2(1);
            m_string.text_param.pivot = glm::vec2(0.5f,0.5f);
            break;
        }
        case TextStype::Dynamic: {
            break;
        }
        default: {
            EFL_LOGGER_ERROR(LogCategory::Font, "Initialize default text param failed: Unknown TextStype");
            break;
        }
    }
    return 0;
}
