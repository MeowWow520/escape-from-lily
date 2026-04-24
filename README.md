## 逃离姛

### 🌐多语言支持
中文（简体） | [英文](./translations/zn-ch/Readme.md)

### 🎮概览

**逃离姛** 是一个逃生游戏，灵感来自于“未知”。

### 📚外部库
| Name          | Version       | Website                                                  |
|---------------|---------------|----------------------------------------------------------|
| SDL3          | release-3.4.2 | [官方百科](https://wiki.libsdl.org/SDL3/FrontPage)        |
| SDL3_image    | release-3.2.4 | [官方百科](https://wiki.libsdl.org/SDL3_image/FrontPage)  |
| SDL3_mixer    | release-3.2.0 | [官方百科](https://wiki.libsdl.org/SDL3_mixer/FrontPage)  |
| SDL3_ttf      | release-3.2.2 | [官方百科](https://wiki.libsdl.org/SDL3_ttf/FrontPage)    |
| glm           | 1.0.1         | [Github](https://github.com/g-truc/glm)                  |
| nlohmann_json | v3.11.3       | [官方](https://json.nlohmann.me/api/ordered_json/)        |

### 📁项目结构

```
📁 EscapeFromGhost    # 项目根目录
├── 📁 assets         # 资源目录
│   ├── 📁 audios           # 音频资源
│   ├── 📁 fonts            # 字体资源
│   ├── 📁 images           # 图片资源
│   │   ├── 📁 backgrounds      # 背景
│   │   ├── 📁 character        # 角色
│   │   ├── 📁 enemy            # 敌人
│   │   ├── 📁 ......
│   ├── 📁 ui               # UI资源
│   ├── 📁......
├── 📁 cmake          # CMake配置文件
├── 📁 docs           # 文档目录
├── 📁 exteral        # 外部库目录
├── 📁 src            # 源代码目录
│   ├── 📁 core             # 核心代码目录
│   ├── main.cpp            # 程序入口文件
│   ├── ......
├── 📁 translations   # 翻译文件目录
│   ├── 📁 en               # 英文翻译文件
│   ├── 📁 zh-CN            # 中文翻译文件
│   ├── 📁 ......
├── CMakeLists.txt    # CMake配置文件
├── LICENSE           # 许可证文件
├── README.md         # 项目说明文件
├── ......
```
