//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_TEXTUREDENTITY_H
#define ESCAPE_FROM_LILY_TEXTUREDENTITY_H
#include "../../Def.h"
#include "../../Object/ObjectScreen.h"


class TexturedEntity : public ObjectScreen {
    protected:

        SDL_Texture *m_texture{};                          // 实体的纹理
        SDL_Color m_colorMod{COLOR(0xFFFFFFFF)};        // 颜色调制 + 透明度
        SDL_BlendMode m_blendMode{SDL_BLENDMODE_BLEND};   // 像素混合模式
        bool m_bTextureDirty{false};                       // 脏标记
        bool m_bVisible{false};
        float m_fRotation{0.0f};                // 旋转角度
        std::string m_strPath{};                // 纹理的文件路径
        SDL_Rect m_rect{};                      // 精灵图矩形
        glm::vec2 m_vec2_textureSize{};         // 纹理原始尺寸
        glm::vec2 m_vec2_scale{1.0f, 1.0f}; // 缩放
        glm::vec2 m_vec2_pivot{0.5f, 0.5f}; // 旋转中心 归一化
        glm::vec2 m_vec2_entityHitbox{1.0f, 1.0f}; // 碰撞箱
    public:
        explicit TexturedEntity(const char* m_entity_name = "TexturedEntity")
            : ObjectScreen(m_entity_name) { }

        ~TexturedEntity() override = default;

        int Initialize() override;
        // setter 和 getter
        SDL_Texture* setTexture(SDL_Texture* new_texture);
        [[nodiscard]] SDL_Texture* getTexture() const;
        SDL_Color setColor(SDL_Color new_color);
        [[nodiscard]] SDL_Color getColorMod() const;
        SDL_BlendMode setBlendMode(SDL_BlendMode new_blend_mode);
        [[nodiscard]] SDL_BlendMode getBlendMode() const;
        bool setDirty(bool new_dirty);
        [[nodiscard]] bool getDirty() const;
        bool setVisible(bool new_visible);
        [[nodiscard]] bool getVisible() const;
        float setRotation(float rotation);
        [[nodiscard]] float getRotation() const;
        std::string setPath(std::string new_path);
        [[nodiscard]] std::string getPath() const;
        SDL_Rect setRect(SDL_Rect new_rect);
        [[nodiscard]] SDL_Rect getRect() const;
        glm::vec2 setTextureSize(glm::vec2 new_texture_size);
        [[nodiscard]] glm::vec2 getTextureSize() const;
        glm::vec2 setScale(glm::vec2 new_scale);
        [[nodiscard]] glm::vec2 getScale() const;
        glm::vec2 setPivot(glm::vec2 new_pivot);
        [[nodiscard]] glm::vec2 getPivot() const;
        glm::vec2 setHitbox(glm::vec2 new_hitbox);
        [[nodiscard]] glm::vec2 getHitbox() const;

        /**
         * TranScreenPos是将世界坐标转化为屏幕坐标的函数
         * @return 屏幕坐标
         */
        [[nodiscard]] glm::vec2 transScreenPos() const;
};

#endif //ESCAPE_FROM_LILY_TEXTUREDENTITY_H
