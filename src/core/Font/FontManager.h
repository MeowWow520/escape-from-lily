//
// Created by MeowWow520 on 2026/5/23.
//

#ifndef ESCAPE_FROM_LILY_FONTMANAGER_H
#define ESCAPE_FROM_LILY_FONTMANAGER_H
#include "../Game.h"
/**
 * FontManager 负责接收来自 Scene 场景的消息，对应更新相应的文本。设置文本渲染的位置、样式等。
 */
class FontManager {
    public:
        // FIXME: 单例存疑
        static FontManager& GetInstance() {
            static FontManager instance;
            return instance;
        }
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
    private:
        FontManager();
        Game& m_game_instance = Game::GetInstance();
};


#endif //ESCAPE_FROM_LILY_FONTMANAGER_H
