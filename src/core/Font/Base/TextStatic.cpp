//
// Created by MeowWow520 on 2026/5/27.
//

#include "TextStatic.h"
#include "../EFL_FontDef.h"
#include "../../Logger/Log.h"


int TextStatic::Initialize() {

    m_text.text_stype = TextStype::Static;
    m_text.font_path = DEFAULT_FONT_PATH;
    m_text.rotation = 0.0f;
    m_text.pivot = glm::vec2(0.5f, 0.5f);
    m_text.scale = glm::vec2(1.0f);
    m_text.is_display_time_out = false;


    m_ttf_font = TTF_OpenFont(m_text.font_path.c_str(), m_text.font_size);
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Font, m_ttf_font, "TTF_OpenFont");
    m_ttf_text = TTF_CreateText(m_font_manager.getTTFEngine(),
        m_ttf_font, m_text.text.c_str(),
        m_text.text.length());
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Font, m_ttf_text, "TTF_CreateText");
    EFL_CHECK_WITH_GET_ERROR(LogCategory::Font,
        TTF_SetTextColor(m_ttf_text,
            m_text.color.r, m_text.color.g,
            m_text.color.b, m_text.color.a),
        "TTF_SetTextColor");

    return 0;
}

void TextStatic::handleEvents(SDL_Event event) {
}

void TextStatic::Update(const float dt) {
    if (m_text.display_time > 0.0f) {
        m_text.display_time -= dt;
        if (m_text.display_time <= 0.0f)
            m_text.is_display_time_out = true;
    }
}

void TextStatic::Render() {
    if (!m_text.is_display_time_out)
        TTF_DrawRendererText(m_ttf_text, m_vec2_screenPos.x, m_vec2_screenPos.y);
}

int TextStatic::Quit() {
    TTF_DestroyText(m_ttf_text);
    return 0;
}
