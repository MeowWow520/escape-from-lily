### 实现思路


#### core/Def.h

`Def.h` 中宏定义了 win64 平台下的 PowerShell 的转义字符，为之后实现的方法提供更易读的格式。如红色：
```c++ 
#define CLR_RED     "\033[31m"
```

宏定义了 HEX 格式的颜色转化为 SDL api 要求的 RGBA 分量。可将 `0xff00ff00` 转化为 `255, 0, 255, 0`。
```c++
#define COLOR(HEX) (\
    ( (HEX) >> 24 ) & 0xFF), \
    (((HEX) >> 16 ) & 0xFF), \
    (((HEX) >> 8  ) & 0xFF), \
    ( (HEX)         & 0xFF)
```

Object 的子类的初始化函数均为返回 int 类型的方法， 且类内定义了成员变量 `m_return_code` 用于返回错误码。`EFL_ClassInit(int, std::source_location)` 方法分别传入返回值和当前语句所在的函数和文件路径。前者传入的 `m_return_code`，当正常运行时，`m_return_code` 被类内 `Initialize()` 方法初始化为 0。`EFL_ClassInit()` 接受到 0，方法内步入打印 `[info]` 日志。而当程序遇到错误时，条件判断语句先将 `m_return_code` 设为错误码，随后 goto 到 return 语句，`EFL_ClassInit()` 接受到非 0，方法内步入打印 `[error]` 日志。由于使用 goto 语句，发生错误后的语句不会运行。程序树一路返回错误码，最终在 mian 函数返回错误码，程序退出。 `EFL_ClassQuit` 同理。
```text
                err -1
EFL_ClassInit()  --->  SceneMain::Initialize() --
                                                | err
                     err -1                     | -1
exe quit  <--  main   <---  Game::Initialize() <-
```

后者传入 std::source_location 类型的数据，为在 return 语句前创建的 `loc` 变量，他记录当前语句所在的函数和文件路径。`EFL_ClassInit()` 方法内对 `loc` 变量这样处理：
**1. file_name()**: 新建字符串 `c_file_name`，他截取并删去路径中 "src" 字符之前的所有字符，此时 location.file_name() 的原值 `G:\programming\cpp\src\core\Entities\Background.cpp` 被转化为 `src\core\Entities\Background.cpp`，随后对 `c_file_name` + 4 删去 "src\" 字符串。最后得到 `core\Entities\Background.cpp`。

**2. line()**: 无特殊转化。

**3. function_name()**: 对原值 + 12 删去了 `int __cdecl ` 字符串得到函数名。`int __cdecl SceneMain::Initialize(void)` 被转化为 `SceneMain::Initialize(void)`。
