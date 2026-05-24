//
// Created by MeowWow520 on 2026/5/24.
//

#include "TextBase.h"

#include "../Def.h"

std::string TextBase::SetText(const std::string& newtext) {
    m_text = newtext;
    return m_text;
}

[[nodiscard]] std::string TextBase::GetText() const {
    return m_text;
}

char TextBase::SetCurrentChar(const char newchar) {
    m_current_char = newchar;
    return m_current_char;
}

[[nodiscard]] char TextBase::GetCurrentChar() const {
    return m_current_char;
}

TextParam TextBase::SetTextParam(const TextParam &newparams) {
    m_text_params = newparams;
    return newparams;
}

[[nodiscard]] TextParam TextBase::GetTextParam() const {
    return m_text_params;
}

int TextBase::InitializeDefaultTextParam() {
    m_text_params.text_stype = TextStype::Static;
    m_text_params.text_display_time = -1;
    m_text_params.font_size = 14;
    m_text_params.text_each_line_width = DEFAULT_TEXT_EACH_LINE_WIDTH;
    m_text_params.color = {DEFAULT_TEXT_COLOR};
    m_text_params.font_path = DEFAULT_FONT_PATH;
    m_text_params.display_backgrd = false;
    return 0;
}
