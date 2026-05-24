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
        std::string m_text{};
        char m_current_char{};
        TextParam m_text_params{};
    public:
        explicit TextBase(const char* m_entity_name = "TextBase")
            : ObjectScreen(m_entity_name) { }

        ~TextBase() override = default;
        // setter 和 getter
        std::string SetText(std::string newtext);
        [[nodiscard]] std::string GetText() const;
        char SetCurrentChar(char newchar);
        [[nodiscard]] char GetCurrentChar() const;
        TextParam SetTextParam(const TextParam &newparams);
        [[nodiscard]] TextParam GetTextParam() const;

        int InitializeDefaultTextParam();
};


#endif //ESCAPE_FROM_LILY_TEXTBASE_H
