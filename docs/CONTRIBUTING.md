# 贡献指南

感谢您对 escape-from-lily 项目的关注！我们欢迎所有形式的贡献，包括但不限于代码、文档、翻译、测试、Bug 报告和功能建议。

## 📋 目录

- [贡献者行为公约](#行为准则)
- [如何贡献](#如何贡献)
- [代码规范](#代码规范)
- [提交流程](#提交流程)
- [翻译贡献](#翻译贡献)
- [报告问题](#报告问题)
- [获取帮助](#获取帮助)

## 🤝 行为准则

在参与本项目时，请遵守我们的[贡献者行为公约](../CODE_OF_CONDUCT.md)。我们致力于为所有人提供一个开放、友好、包容的社区环境。

## 🚀 如何贡献

### 快速开始

1. Fork 本仓库
2. 创建您的特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交您的更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启一个 Pull Request

## 📝 代码规范

### C++ 代码风格

> 更多代码风格详见[Coding.md](./Coding.md)

- 使用 **C++17** 标准
- 遵循 **Google C++ Style Guide**（主要规范）
- 使用 4 空格缩进（不使用制表符）
- 类名使用 PascalCase（如 `GameEngine`）
- 变量名使用 camelCase（如 `gameEngine`）
- 常量名使用 UPPER_SNAKE_CASE（如 `MAX_PLAYERS`）
- 私有成员变量以 `_` 结尾（如 `playerCount_`）


### 注释规范

```cpp
/**
 * @brief 函数的简要描述
 *
 * @param param_name 参数描述
 * @return 返回值描述
 */
int calculateScore(int param_name);
```

### 文件组织

- 头文件 (`.h`)：只包含声明
- 源文件 (`.cpp`)：包含实现
- 每个类对应一个头文件和一个源文件
- 文件名与类名保持一致

## 🔄 提交流程

### Commit 消息格式

使用 [Conventional Commits](https://www.conventionalcommits.org/) 规范：

```
<type>(<scope>): message
```

**类型（type）**:
- `upd`: 更新进度
- `feat`: 新功能
- `fix`: Bug 修复
- `docs`: 文档更新
- `style`: 代码格式（不影响代码运行）
- `refactor`: 重构（既不是新功能也不是修复）
- `perf`: 性能优化
- `test`: 测试相关
- `chore`: 构建过程或辅助工具的变动

**示例**:
```
feat(player): add character movement system

Implement smooth player movement with collision detection.
Include keyboard input handling and velocity-based movement.

Closes #123
```

### Pull Request 流程

1. **更新主分支**
   ```bash
   git fetch upstream
   git checkout main
   git merge upstream/main
   ```

2. **创建功能分支**
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **开发并提交**
   ```bash
   # 进行开发
   git add .
   git commit -m "feat: add your feature description"
   ```

4. **推送分支**
   ```bash
   git push origin feature/your-feature-name
   ```

5. **创建 Pull Request**
   - 填写 PR 模板
   - 描述更改内容
   - 关联相关 Issue
   - 添加必要的截图或演示

### PR 检查清单

在提交 PR 前，请确保：

- [ ] 代码遵循项目规范
- [ ] 添加了必要的测试
- [ ] 所有测试通过
- [ ] 更新了相关文档
- [ ] Commit 消息清晰明确
- [ ] PR 描述完整
- [ ] 没有引入新的警告

## 🌍 翻译贡献

我们支持多语言，欢迎您参与翻译工作！

### 如何参与翻译

1. **选择语言**
   - 查看 `translations/` 目录下已有的语言版本
   - 选择您要翻译或改进的语言

2. **创建新翻译**
   ```bash
   # 复制英文版本作为模板
   cp translations/en/README.md translations/your-lang/README.md

   # 或创建新的语言目录
   mkdir translations/your-lang
   ```

3. **翻译内容**
   - 翻译 README.md 和其他文档文件
   - 保持格式和结构一致
   - 确保技术术语准确

4. **提交翻译**
   ```bash
   git add translations/your-lang/
   git commit -m "docs: add your-language translation"
   ```

### 翻译注意事项

- 保持原文的技术准确性
- 使用专业的术语
- 考虑文化差异
- 测试翻译后的显示效果
- 与其他翻译者保持沟通

## 🐛 报告问题

### Bug 报告

报告 Bug 时，请提供以下信息：

1. **环境信息**
   - 操作系统版本
   - 编译器版本
   - 项目版本或 commit hash

2. **问题描述**
   - 清晰描述遇到的问题
   - 提供重现步骤
   - 说明预期行为和实际行为

3. **日志和截图**
   - 附加相关的错误日志
   - 提供截图或录屏
   - 如果是编译问题，提供完整的编译输出

### 功能建议

提出新功能建议时：

1. 详细描述功能需求
2. 说明使用场景
3. 分析实现的可行性
4. 如有可能，提供设计草图或参考

**Bug 报告模板**:
```markdown
**描述**
简短描述遇到的问题

**重现步骤**
1. 第一步
2. 第二步
3. ...

**预期行为**
描述您期望发生什么

**实际行为**
描述实际发生了什么

**环境**
- OS: [例如 Windows 11]
- 编译器: [例如 MSVC 2022]
- 版本: [例如 v0.1.0]

**附加信息**
添加任何其他有用的信息、截图等
```

## 💬 获取帮助

如果您在贡献过程中遇到问题：

1. **查看文档**: 首先查看项目文档和现有的 Issue
2. **提问**: 创建一个 Issue，标签设为 `question`
3. **讨论**: 在 PR 中与维护者和其他贡献者讨论
4. **邮件**: 联系项目维护者

## 📚 学习资源

如果您想了解更多：

- [SDL3 官方文档](https://wiki.libsdl.org/SDL3/)
- [CMake 教程](https://cmake.org/cmake/help/latest/guide/tutorial/)
- [现代 C++ 最佳实践](https://github.com/isocpp/CppCoreGuidelines)
- [Git 工作流程](https://www.atlassian.com/git/tutorials/comparing-workflows)

## ⭐ 认可贡献者

所有贡献者都将被添加到 [CONTRIBUTORS](./CONTRIBUTORS.md) 列表中。我们感谢每一位为项目做出贡献的人！

---

再次感谢您的贡献！让我们一起把 escape-from-lily 变得更好！🎮✨