//
// Created by MeowWow520 on 2026/5/19.
//

#ifndef ESCAPE_FROM_LILY_LOGCATEGORY_H
#define ESCAPE_FROM_LILY_LOGCATEGORY_H


enum class LogCategory {
    Core,    // 游戏类，SDL 子系统，窗口渲染，应用退出
    Entity,  // Object 构造/析构，实体的 Initialize()/Quit()
    Input,   // 键盘/鼠标的输入事件
    Scene,   // 场景切换，状态改变
    Renderer // 纹理加载，渲染
};

#endif //ESCAPE_FROM_LILY_LOGCATEGORY_H
