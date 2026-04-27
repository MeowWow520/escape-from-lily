## ##对各个类的解释



### ###class Game
定义在 `src/core/Game.h`。单例模式

**Game**类是游戏实例运行的主类。它负责创建窗口设置渲染器，初始化库。


### ###class Object
定义在 `src/core/Object.h`。基类

游戏资源的最小单位[^1]？拥有基础的**初始化(init)**、**事件处理(handleEvents)**、**更新(Update)**、**渲染(Render)**、**清理(Clean)** 等函数。并且定义了单例 Game 的引用，使其派生类可以访问 Game。


### ###class Scene
定义在 `src/core/Scene.h`。场景类，继承自 `Object`


### ###class ObjectWorld
定义在 `src/core/ObjectWorld.h`。

为 Object 添加世界坐标这一成员，并提供 `getter` 和 `setter`。


### ###class ObjectScreen
定义在 `src/core/ObjectScreen.h`。

为 Object 添加屏幕渲染坐标这一成员，并提供 `getter` 和 `setter`。


[^1]: 存疑