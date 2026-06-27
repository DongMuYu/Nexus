# Nexus

实时物理引擎，专注于粒子系统模拟与刚体动力学。基于 C++17 和 SFML 构建，集成 Dear ImGui 实现实时交互与参数调试。

## 功能特性

- **粒子系统** — 支持点发射器、区域发射器、鼠标交互发射器，可实时调控粒子数量、生命周期、速度等参数
- **物理模拟** — 多种数值积分方法、空间哈希/四叉树加速碰撞检测、约束求解
- **多线程** — 基于线程池的并行物理计算
- **实时调试** — Dear ImGui 控制面板，支持运行时参数热调节
- **调试可视化** — 碰撞边界、速度向量、空间分区等调试绘制
- **后处理效果** — 可扩展的后处理管线
- **性能分析** — 内置 Profiler，实时显示帧率与各阶段耗时

## 技术栈

| 组件 | 技术 |
|------|------|
| 语言 | C++17 |
| 渲染 | SFML 2.5.1 + OpenGL |
| UI | Dear ImGui + ImGui-SFML |
| 构建 | CMake 3.10+ |
| 平台 | Windows (MSVC) |

## 项目结构

```
Nexus/
├── src/
│   ├── core/           # 核心系统：Application、Config、Profiler、ThreadPool
│   ├── physics/        # 物理引擎：World、Solver、SpatialHash
│   ├── particles/      # 粒子系统：ParticleSystem、CoreParticle、Emitters
│   ├── rendering/      # 渲染：Renderer、DebugDraw、PostProcess
│   ├── ui/             # UI：UIManager (Dear ImGui)
│   └── utils/          # 工具：Math、Logger、FileIO
├── docs/
│   └── ARCHITECTURE.md # 架构设计文档
├── scripts/            # 构建与运行脚本
├── test/               # 测试
└── CMakeLists.txt
```

## 构建

### 依赖

- CMake 3.10+
- Visual Studio 2019+ (MSVC)
- [SFML 2.5.1](https://www.sfml-dev.org/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [ImGui-SFML](https://github.com/SFML/imgui-sfml)

### 步骤

1. 安装 SFML 2.5.1 至 `D:/Libraries/SFML-2.5.1`
2. 克隆 ImGui 至 `D:/Libraries/imgui`
3. 克隆 ImGui-SFML 至 `D:/Libraries/imgui-sfml`，并用 CMake 编译
4. 构建项目：

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## 运行

```bash
./build/Release/Nexus.exe
```

启动后，通过 ImGui 面板调整粒子参数，使用鼠标直接与粒子系统交互。

## 架构概览

```
输入处理 → 物理模拟 → 渲染 → UI 更新
              ↓
   空间分区 → 碰撞检测 → 约束求解 → 状态更新
```

更多细节见 [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)。

## 许可

MIT License
