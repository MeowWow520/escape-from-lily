## escape-from-lily

> [!TIP]
> 本仓库由初学者维护，可能存在大量错误


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
4: 添加 CMake 选项
```text
-DCMAKE_TOOLCHAIN_FILE=Path\you\known\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_FEATURE_FLAGS=manifests -DVCPKG_TARGET_TRIPLET=x64-windows-static
```
5: 构建项目

#### 其他平台
前往 --> [Build.md](docs/Build.md)
