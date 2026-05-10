> [!TIP]
> 最后更改的日期：2025/5/10 By [MeowWow520](https://meowwow520.cn/)
>
> 
## CLASS

> [!NOTE]
> 本文件对本项目的所有**类**进行描述
> 
> 包括 **继承关系** 和 **数据结构**

### class Object
`Object` 提供一个对象基础的功能，访问 game_instance 单例。

具有基本的`Initialize()`,`HandleEvents()`、`Update()`、`Render()`、`Quit()`方法

### class Scene
`Scene` 继承自 `Object` 为对象提供相机位置坐标，世界大小。

相机位置坐标为屏幕左上角坐标在世界坐标系下的位置

世界大小将与子类定义的 `world_scale_` 与屏幕大小相乘得到

### class TODO



