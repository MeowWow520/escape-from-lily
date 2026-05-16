//
// Created by MeowWow520 on 2026/5/15.
//

#ifndef ESCAPE_FROM_LILY_ACTION_H
#define ESCAPE_FROM_LILY_ACTION_H


enum class Action : int {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Pause, Quit
};

enum class ActionState : int {
    Idle,     // 无操作
    Pressed,  // 按下（上升沿）
    Held,     // 按住中
    Released  // 松开（下降沿）
};





#endif //ESCAPE_FROM_LILY_ACTION_H
