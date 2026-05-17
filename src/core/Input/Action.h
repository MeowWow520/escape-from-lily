//
// Created by MeowWow520 on 2026/5/15.
//

#ifndef ESCAPE_FROM_LILY_ACTION_H
#define ESCAPE_FROM_LILY_ACTION_H


enum class Action : int {
    MoveUp,    // 向上移动
    MoveDown,  // 向下移动
    MoveLeft,  // 向左移动
    MoveRight, // 向右移动
    Pause,     // 暂停游戏
    Quit       // 退出游戏
};

enum class ActionState : int {
    Idle,     // 无操作
    Pressed,  // 按下（上升沿）
    Held,     // 按住中
    Released  // 松开（下降沿）
};





#endif //ESCAPE_FROM_LILY_ACTION_H
