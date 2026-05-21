## escape-from-lily

> [!TIP]
> 本仓库由初学者维护，可能存在大量错误

[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/MeowWow520/escape-from-lily) | ![GitHub License](https://img.shields.io/github/license/MeowWow520/escape-from-lily)

### 介绍
**这是什么？**
这是一款基于 `C++` 和 `SDL3` 的逃离游戏

**Features:**
- 基于 `C++20` 标准
- 使用 `CMake` 构建
- 使用 `vcpkg` 管理依赖
- 使用 `SDL3` 作为图形库
### 参与贡献

#### CLion
1: 拉取仓库
```bash
git clone https://github.com/MeowWow520/escape-from-lily.git
```
2: 在 [CLion](https://www.jetbrains.com/clion/) 中打开项目

3: 安装 [vcpkg](https://vcpkg.io) 包管理工具
```bash
cd Path\you\known
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```
4: 添加 CMake 选项 <font color="blue">注意文件路径</font>
```textmate
-DCMAKE_TOOLCHAIN_FILE=Path\you\known\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_FEATURE_FLAGS=manifests -DVCPKG_TARGET_TRIPLET=x64-windows-static
```
5: 构建项目

#### 其他平台
前往 --> [Build.md](docs/Build.md)

### 文件结构速览

```
📁 escape-from-lily          # 项目根目录
├── 📁 .github               # GitHub Actions 配置
│   └── 📁 workflows         # CI 工作流
│       └── ci-windows.yml
├── 📁 assets                # 资源目录
│   └── 📁 images            # 图片资源
│       ├── draft_Background_final.png
│       └── test.jpg
├── 📁 docs                  # 文档目录
│   ├── Build.md
│   ├── Class.docs.md
│   ├── CONTRIBUTING.md
│   ├── CONTRIBUTORS.md
│   ├── FileStructure.md
│   ├── implement.md
│   ├── 📁 implements        # 实现参考
│   └── TODO.md
├── 📁 draft                 # 草稿/原型资源
├── 📁 src                   # 源代码目录
│   ├── 📁 core              # 核心代码
│   │   ├── 📁 Entities      # 实体类
│   │   │   ├── 📁 Base      # 实体基类
│   │   │   │   ├── MovableEntity.h / .cpp
│   │   │   │   └── TexturedEntity.h / .cpp
│   │   │   ├── Background.h / .cpp
│   │   │   ├── Camera.h / .cpp
│   │   │   ├── Player.h / .cpp
│   │   │   └── UserInterface.h / .cpp
│   │   ├── 📁 Input         # 输入处理
│   │   │   ├── Action.h
│   │   │   ├── Input.h / .cpp
│   │   │   └── KeyboardInput.h / .cpp
│   │   ├── 📁 Logger        # 日志系统
│   │   │   ├── Log.h / .cpp
│   │   │   └── LogCategory.h
│   │   ├── 📁 Object        # 对象基类
│   │   │   ├── Object.h / .cpp
│   │   │   ├── ObjectScreen.h
│   │   │   └── ObjectWorld.h
│   │   ├── Def.h
│   │   ├── Game.h / .cpp
│   │   └── Scene.h
│   ├── main.cpp             # 程序入口
│   ├── SceneMain.h / .cpp   # 主场景
│   └── SceneTitle.h / .cpp  # 标题场景
├── .gitignore
├── AGENTS.md
├── CMakeLists.txt           # CMake 构建配置
├── CMakePresets.json        # CMake 预设
├── CODE_OF_CONDUCT.md
├── icon.ico
├── 📁 innosetup              # Inno Setup 安装程序
├── LICENSE                  # 许可证文件
├── qodana.yaml
├── Readme.md                # 项目说明
└── vcpkg.json               # vcpkg 依赖清单
```

> 完整版请参阅 [docs/FileStructure.md](docs/FileStructure.md)
