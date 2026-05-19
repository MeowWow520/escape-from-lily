//
// Created by MeowWow520 on 2026/5/11.
//

#ifndef ESCAPE_FROM_LILY_BACKGROUND_H
#define ESCAPE_FROM_LILY_BACKGROUND_H
#include "Base/TexturedEntity.h"


class Background : public TexturedEntity {
    public:
        explicit Background(const char* m_entity_name = "Background")
            : TexturedEntity(m_entity_name) {}

        /**
         * 初始化 Background，应初始化背景文件的资源地址，调整不可见为可见，设置大小
         *
         * @return 初始化成功返回 true，失败返回 false
         */
        int Initialize() override;

        /**
         * 更新函数，应在这里更新 Background 的世界坐标
         *
         * @param dt delta time
         */
        void Update(float dt) override;

        /**
         * 渲染 Background，请根据世界坐标和相机坐标调整渲染的屏幕位置和渲染的矩形
         */
        void Render() override;

        /**
         * 清理 Background 并退出。
         *
         * @return 清理成功返回 true，失败返回 false
         */
        int Quit() override;

};


#endif //ESCAPE_FROM_LILY_BACKGROUND_H
