//
// Created by MeowWow520 on 2026/5/10.
//

#ifndef ESCAPE_FROM_LILY_TEXTUREDENTITY_H
#define ESCAPE_FROM_LILY_TEXTUREDENTITY_H
#include <memory>
#include "../../Def.h"
#include "../../Object/ObjectScreen.h"


class TexturedEntity : public ObjectScreen {
    protected:

        SDL_Texture *m_texture{};                          // 实体的纹理
        SDL_Color m_color_mod{COLOR(0xFFFFFFFF)};        // 颜色调制 + 透明度
        SDL_BlendMode m_blend_mode{SDL_BLENDMODE_BLEND};   // 像素混合模式
        bool m_texture_dirty{false};                       // 脏标记
        bool m_visible{false};
        float m_rotation{0.0f};            // 旋转角度
        std::string m_path{};              // 纹理的文件路径
        SDL_Rect m_rect{};                 // 精灵图矩形
        glm::vec2 m_texture_size{};        // 纹理原始尺寸
        glm::vec2 m_scale{1.0f, 1.0f}; // 缩放
        glm::vec2 m_pivot{0.5f, 0.5f}; // 旋转中心 归一化
        glm::vec2 m_entity_hitbox{1.0f, 1.0f}; // 碰撞箱
    public:
        explicit TexturedEntity(const char* m_entity_name = "TexturedEntity")
            : ObjectScreen(m_entity_name) { }

        ~TexturedEntity() override = default;

        int Initialize() override;
        // setter 和 getter
        SDL_Texture* SetTexture(SDL_Texture* newtexture) noexcept;
        [[nodiscard]] SDL_Texture* GetTexture() const;
        SDL_Color SetColor(SDL_Color newcolor);
        [[nodiscard]] SDL_Color GetColorMod() const;
        SDL_BlendMode SetBlendMode(SDL_BlendMode newblendmode);
        [[nodiscard]] SDL_BlendMode GetBlendMode() const;
        bool SetDirty(bool newdirty);
        [[nodiscard]] bool GetDirty() const;
        bool SetVisible(bool newvisible);
        [[nodiscard]] bool GetVisible() const;
        float SetRotation(float rotation);
        [[nodiscard]] float GetRotation() const;
        std::string SetPath(std::string newpath);
        [[nodiscard]] std::string GetPath() const;
        SDL_Rect SetRect(SDL_Rect newrect);
        [[nodiscard]] SDL_Rect GetRect() const;
        glm::vec2 SetTextureSize(glm::vec2 newtexturesize);
        [[nodiscard]] glm::vec2 GetTextureSize() const;
        glm::vec2 SetScale(glm::vec2 newscale);
        [[nodiscard]] glm::vec2 GetScale() const;
        glm::vec2 SetPivot(glm::vec2 newpivot);
        [[nodiscard]] glm::vec2 GetPivot() const;
        glm::vec2 SetHitbox(glm::vec2 newhitbox);
        [[nodiscard]] glm::vec2 GetHitbox() const;

        /**
         * InitializeTextureFromPath 读取成员 m_path 的值，将 m_path 指向的文件作为纹理渲染，将该文件进行初始化。并将是否显示设为允许
         *
         * @return 初始化成功返回 true，失败返回 false
         */
        bool InitializeTextureFromPath();

        /**
         * SetTextureFromPath 读取成员 m_path 的值，将 m_path 指向的文件作为纹理赋值到 m_texture
         *
         * @return 初始化成功返回 true，失败返回 false
         */
        bool SetTextureFromPath();

        /**
         * TranScreenPos是将世界坐标转化为屏幕坐标的函数
         * @return 屏幕坐标
         */
        [[nodiscard]] glm::vec2 TransScreenPos() const;
        // FIXME: 无用的函数？
        [[nodiscard]] bool IsInCameraRange() const;
};

#endif //ESCAPE_FROM_LILY_TEXTUREDENTITY_H
