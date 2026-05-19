### 文件结构

```
📁 escape-from-lily          # 项目根目录
├── 📁 .github               # GitHub Actions 配置
│   └── 📁 workflows         # CI 工作流目录
│       └── ci-windows.yml   # Windows CI 工作流
├── 📁 assets                # 资源目录
│   └── 📁 images            # 图片资源
│       ├── draft_Background_final.png
│       └── test.jpg
├── 📁 docs                  # 文档目录
│   ├── Build.md             # 构建指南
│   ├── Class.docs.md        # 类文档
│   ├── CONTRIBUTING.md      # 贡献指南
│   ├── CONTRIBUTORS.md      # 贡献者列表
│   ├── FileStructure.md     # 文件结构（本文件）
│   ├── implement.md         # 实现文档
│   ├── 📁 implements         # 实现参考文档
│   │   ├── opencode_20260514_how-to-implement-input.md  # 输入系统设计
│   │   └── opencode_20260514_sdl3-events.md             # SDL3 事件参考
│   └── TODO.md              # 待办事项
├── 📁 draft                 # 草稿/原型资源
│   ├── draft_Background_final.png   # 背景终稿
│   ├── draft_Background_struct.png  # 背景结构图
│   └── HHsxx1OaQAARJqs.png          # 草稿图片
├── 📁 src                   # 源代码目录
│   ├── 📁 core              # 核心代码
│   │   ├── 📁 Entities      # 实体类
│   │   │   ├── 📁 Base      # 实体基类
│   │   │   │   ├── MovableEntity.h / .cpp   # 可移动实体
│   │   │   │   └── TexturedEntity.h / .cpp  # 带纹理实体
│   │   │   ├── Background.h / .cpp          # 背景
│   │   │   ├── Camera.h / .cpp              # 相机
│   │   │   ├── Player.h / .cpp              # 玩家
│   │   │   └── UserInterface.h / .cpp       # 用户界面
│   │   ├── 📁 Input         # 输入处理
│   │   │   ├── Action.h                      # 动作/状态枚举
│   │   │   ├── Input.h / .cpp               # 输入抽象
│   │   │   └── KeyboardInput.h / .cpp       # 键盘输入
│   │   ├── 📁 Logger        # 日志系统
│   │   │   ├── Log.h / .cpp                  # 日志宏与配置
│   │   │   └── LogCategory.h                # 日志分类枚举
│   │   ├── 📁 Object        # 对象基类
│   │   │   ├── Object.h / .cpp              # Object 基类
│   │   │   ├── ObjectScreen.h               # 屏幕坐标对象
│   │   │   └── ObjectWorld.h                # 世界坐标对象
│   │   ├── Def.h            # 通用定义/宏
│   │   ├── Game.h / .cpp    # 游戏主循环
│   │   └── Scene.h          # 场景基类
│   ├── main.cpp             # 程序入口
│   ├── SceneMain.h / .cpp   # 主场景
│   └── SceneTitle.h / .cpp  # 标题场景
├── .gitignore               # Git 忽略规则
├── AGENTS.md                # AI Agent 配置
├── CMakeLists.txt           # CMake 构建配置
├── CMakePresets.json        # CMake 预设
├── CODE_OF_CONDUCT.md       # 行为准则
├── icon.ico                  # 应用图标
├── 📁 innosetup              # Inno Setup 安装程序
│   ├── 1.00.0.iss
│   └── 📁 informations
├── LICENSE                  # 许可证文件
├── qodana.yaml              # Qodana 配置
├── Readme.md                # 项目说明
└── vcpkg.json               # vcpkg 依赖清单
```
