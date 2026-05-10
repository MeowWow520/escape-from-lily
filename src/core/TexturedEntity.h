//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_TEXTUREDENTITY_H
#define ESCAPE_FROM_LILY_TEXTUREDENTITY_H
#include <memory>
#include "Def.h"
#include "ObjectScreen.h"



class TexturedEntity : public ObjectScreen {
    protected:
        // 实体的纹理
        std::unique_ptr<
            SDL_Texture,
            decltype(&SDL_DestroyTexture)
        > m_texture{nullptr, SDL_DestroyTexture};
        SDL_Rect m_rect{};                  // 精灵图矩形
        glm::vec2 m_texture_size{};         // 纹理原始尺寸
        glm::vec2  m_scale{1.0f, 1.0f}; // 缩放
        glm::vec2  m_pivot{0.5f, 0.5f}; // 旋转中心 归一化
        float m_rotation = 0.0f;            // 旋转角度

        // 5. 视觉控制
        SDL_Color m_colorMod{COLOR(0xFFFFFFFF)};       // 颜色调制 + 透明度
        SDL_BlendMode m_blendMode = SDL_BLENDMODE_BLEND; // 像素混合模式
        bool m_textureDirty = false;                     // 脏标记
        bool m_visible = true;

    public:
        TexturedEntity();
        ~TexturedEntity() override = default;

};

#endif //ESCAPE_FROM_LILY_TEXTUREDENTITY_H
