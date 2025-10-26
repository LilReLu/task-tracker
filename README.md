task-trackertask-tracker is a simple, command-line (CLI) task management tool written in C++20 [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/CMakeLists.txt, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/CMakeLists.txt].It provides an interactive REPL (Read-Eval-Print Loop) to add, remove, update, and list your tasks. All tasks are automatically persisted to a local tasks.json file, ensuring your data is saved between sessions [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/task_manager.cpp].✨ FeaturesAdd Task: Add a new task with a description [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].List Tasks: Show all tasks, or filter by status (TO_DO, IN_PROGRESS, DONE) [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].Get Task: View details for a single task by its ID [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].Update Task: Update a task's description or status by ID [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].Remove Task: Delete a task by its ID [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].Remove Last: Remove the most recently added task [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].Clear All: Deletes all tasks from storage (with a safety confirmation) [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].Persistent Storage: All changes are immediately saved to tasks.json [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/task_manager.cpp].🛠️ Tech StackLanguage: C++20 [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/CMakeLists.txt, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/CMakeLists.txt]Build System: CMake [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/CMakeLists.txt]Package Manager: vcpkg (via vcpkg.json manifest) [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/vcpkg.json, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/CMakePresets.json]Core Libraries:nlohmann/json: For JSON serialization/deserialization (task persistence) [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/vcpkg.json, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/task_manager.cpp].cxxopts: For parsing command-line arguments in the REPL [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/vcpkg.json, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].GoogleTest: For unit testing [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/vcpkg.json, lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/tests/CMakeLists.txt].🚀 Getting StartedPrerequisitesCMake (3.15+) [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/CMakeLists.txt]A C++20 compliant compiler (e.g., MSVC, GCC, Clang)vcpkg (for installing dependencies)Build & RunClone the repository:git clone [https://github.com/your-username/task-tracker.git](https://github.com/your-username/task-tracker.git)
cd task-tracker
Install dependencies using vcpkg:vcpkg will automatically read the vcpkg.json file and install the required libraries.vcpkg install
Configure CMake:This project uses CMakePresets.json [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/CMakePresets.json] for easy configuration. Make sure to replace <path-to-vcpkg> with the path to your vcpkg installation.# Example for Windows (using the preset)
cmake --preset=x64-debug -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake

# Example for Linux/macOS (manual)
# cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug
Build the project:# Using the preset
cmake --build out/build/x64-debug

# Or manually
# cmake --build build
Run the application:# Windows
./out/build/x64-debug/task-tracker.exe

# Linux/macOS
# ./build/task-tracker
⌨️ How to UseOnce started, the application enters an interactive REPL mode, indicated by the > prompt. Type help to see all commands, or exit/quit to leave [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/srcB/main.cpp].All commands are parsed using cxxopts [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/src/main.cpp].help or h: Prints the help message.add <desc> or a <desc>: Adds a new task. Use quotes for descriptions with spaces (e.g., add "My new task").list or l: Lists all tasks (this is the default action if no command is given).list --status <STATUS>: Filters the list by status. <STATUS> can be TO_DO, IN_PROGRESS, or DONE.get <ID> or g <ID>: Gets a single task by its ID.remove <ID> or r <ID>: Removes a task by its ID.r-last: Removes the most recently added task.update <ID> [--desc <text>] [--status <STATUS>]: Updates a task. You must provide at least one of --desc or --status.clear or c: Clears all tasks from storage (requires 'y' confirmation).exit or quit: Exits the program.Example SessionWelcome to the interactive task manager.
Type 'help' for commands, 'exit' or 'quit' to exit.
---
> add "Learn C++"
Task added successfully:
-------------------------
Task ID: 1
Description: Learn C++
Status: TO_DO
Created At: Sun Oct 26 21:30:00 2025
Updated At: Sun Oct 26 21:30:00 2025
-------------------------

> add "Write README file"
Task added successfully:
-------------------------
Task ID: 2
Description: Write README file
Status: TO_DO
Created At: Sun Oct 26 21:30:15 2025
Updated At: Sun Oct 26 21:30:15 2025
-------------------------

> list
-------------------------
Task ID: 1
Description: Learn C++
Status: TO_DO
...
-------------------------
------------------------
Task ID: 2
Description: Write README file
Status: TO_DO
...
-------------------------

> update 2 --status "IN_PROGRESS"
Task updated successfully:
-------------------------
Task ID: 2
Description: Write README file
Status: IN_PROGRESS
...
Updated At: Sun Oct 26 21:30:30 2025
-------------------------

> r 1
Task with ID 1 removed successfully.

> quit
Exiting...
🧪 Running TestsThis project includes a comprehensive test suite using GoogleTest [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/vcpkg.json]. The tests cover the core Task logic [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/tests/task.cpp], the TaskManager functionality (including file I/O) [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/tests/manager.cpp], and the UI print functions [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/tests/ui.cpp].To build and run the tests, execute the run_tests target generated by CMake [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/tests/CMakeLists.txt]:# Windows
./out/build/x64-debug/tests/run_tests.exe

# Linux/macOS
# ./build/tests/run_tests
📄 LicenseThis project is licensed under the MIT License [cite: lilrelu/task-tracker/task-tracker-38268c593b383a3da9c5453af831dda56f8089ec/LICENSE].