task-tracker
task-tracker 是一个基于 C++20 开发的简单命令行任务管理工具。

它提供一个交互式的 REPL (Read-Eval-Print Loop) 界面，用于添加、删除、更新和列出任务。所有任务都会被自动持久化存储在本地的 tasks.json 文件中。

✨ 主要功能
添加任务: 添加一个新的待办事项。

列出任务: 显示所有任务，或按状态 (TO_DO, IN_PROGRESS, DONE) 筛选。

获取任务: 按 ID 查看单个任务的详细信息。

更新任务: 按 ID 更新任务的描述或状态。

删除任务: 按 ID 删除一个任务。

删除最后任务: 移除最近添加的一个任务。

清空所有: 删除所有任务（有安全确认）。

持久化存储: 所有更改都会立即保存到 tasks.json 文件。

🛠️ 技术栈
语言: C++20

构建系统: CMake

包管理: vcpkg (通过 vcpkg.json 清单管理)

核心库:

nlohmann/json: 用于 JSON 的读写（任务持久化）。

cxxopts: 用于解析 REPL 中的命令行参数。

GoogleTest: 用于单元测试。

🚀 开始使用
依赖环境
CMake (3.15+)

支持 C++20 的编译器 (例如 MSVC, GCC, Clang)

vcpkg (用于安装依赖)

构建与运行
克隆仓库

Bash

git clone https://github.com/your-username/task-tracker.git
cd task-tracker
使用 vcpkg 安装依赖 vcpkg 会自动读取 vcpkg.json 文件并安装所需的依赖项。

Bash

vcpkg install
配置 CMake 项目使用了 CMakePresets.json，可以轻松配置。确保替换 <path-to-vcpkg> 为你本地 vcpkg 的路径。

Bash

# 示例 (Windows, 使用 VS 2022)
cmake --preset=x64-debug -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake

# 示例 (Linux/macOS)
# cmake -B build -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
构建项目

Bash

# 使用预设
cmake --build out/build/x64-debug

# 传统方式
# cmake --build build
运行程序

Bash

# Windows
./out/build/x64-debug/task-tracker.exe

# Linux/macOS
# ./build/task-tracker
(可选) 运行测试 项目包含了完整的单元测试。

Bash

# Windows
./out/build/x64-debug/tests/run_tests.exe

# Linux/macOS
# ./build/tests/run_tests
⌨️ 如何使用
程序启动后，你将进入一个交互式提示符 >。输入 help 查看所有可用命令，输入 exit 或 quit 退出程序。

命令格式 (基于 cxxopts)

help 或 h: 打印帮助信息。

add <描述> 或 a <描述>: 添加一个新任务。如果描述包含空格，请使用引号，例如 add "我的新任务"。

list 或 l: 列出所有任务 (这是默认操作，直接按回车也会触发)。

list --status <状态>: 按状态 (TO_DO, IN_PROGRESS, DONE) 筛选任务。

get <ID> 或 g <ID>: 按 ID 获取特定任务。

remove <ID> 或 r <ID>: 按 ID 删除任务。

r-last: 删除最后添加的任务。

update <ID> [--desc <新描述>] [--status <新状态>]: 更新任务。必须至少提供 --desc 或 --status 中的一个。

clear 或 c: 清除所有任务 (需要二次确认)。

exit 或 quit: 退出程序。

示例会话
欢迎使用交互式任务管理器。
输入 'help' 查看命令, 'exit' 或 'quit' 退出。
---
> add "学习 C++"
Task added successfully:
-------------------------
Task ID: 1
Description: 学习 C++
Status: TO_DO
...
-------------------------

> add "编写 README 文件"
Task added successfully:
-------------------------
Task ID: 2
Description: 编写 README 文件
Status: TO_DO
...
-------------------------

> list
-------------------------
Task ID: 1
Description: 学习 C++
Status: TO_DO
...
-------------------------
------------------------
Task ID: 2
Description: 编写 README 文件
Status: TO_DO
...
-------------------------

> update 2 --status "IN_PROGRESS"
Task updated successfully:
-------------------------
Task ID: 2
Description: 编写 README 文件
Status: IN_PROGRESS
...
-------------------------

> r 1
Task with ID 1 removed successfully.

> quit
正在退出...
📄 许可证
本项目基于 MIT 许可证。