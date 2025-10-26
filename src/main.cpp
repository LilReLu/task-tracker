#include "cxxopts.hpp"         
#include "task_manager.h"    
#include "ui.h"             
#include "task.h"           
#include <iostream>
#include <string>
#include <vector>
#include <sstream>           // 用于 std::istringstream
#include <iomanip>           // 用于 std::quoted (处理带空格的字符串)

int main() {
    //  初始化 TaskManager (只一次)
    TaskManager manager("tasks.json");

    // 设置 cxxopts (只一次)
    // 用 cxxopts 来解析用户在 REPL 中输入的每一行
    cxxopts::Options options("task-cli",
        "一个 C++ 任务管理器。\n"
        "在 > 提示符后输入命令选项 (例如 --add \"新任务\")");

    options.add_options()
        ("l,list", "列出所有任务 (默认操作)")
        ("g,get", "按 ID 获取单个任务", cxxopts::value<int>(), "<任务ID>")
        ("a,add", "添加一个新任务", cxxopts::value<std::string>(), "<任务描述>")
        ("r,remove", "按 ID 删除一个任务", cxxopts::value<int>(), "<任务ID>")
        ("u,update", "按 ID 更新一个任务 (必须配合 --desc 或 --status)", cxxopts::value<int>(), "<任务ID>")
        ("d,desc", "设置新的任务描述 (配合 --update)", cxxopts::value<std::string>())
        ("s,status", "设置新的任务状态 (TO_DO, IN_PROGRESS, DONE)", cxxopts::value<std::string>())
        ("h,help", "打印帮助信息");

    // 打印欢迎信息
    std::cout << "欢迎使用交互式任务管理器。" << std::endl;
    std::cout << "输入 'help' 查看命令, 'exit' 或 'quit' 退出。" << std::endl;
    std::cout << "---" << std::endl;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break; // EOF (例如 Ctrl+D 或 Ctrl+Z)
        }
        if (line.empty()) {
            continue;
        }

        // 将用户输入 (std::string) 转换为 (argc, char**) 以便 cxxopts 解析
        std::istringstream iss(line);
        std::vector<std::string> args;
        std::string arg;

        // 使用 std::quoted 来正确处理带引号的字符串
        while (iss >> std::quoted(arg)) {
            args.push_back(arg);
        }

        if (args.empty()) {
            continue;
        }

        // 检查 REPL 的特殊命令
        if (args[0] == "exit" || args[0] == "quit") {
            break;
        }
        if (args[0] == "help") {
            // 如果用户输入 'help'，手动显示 cxxopts 的帮助信息
            std::cout << options.help() << std::endl;
            continue;
        }

        // --- Cxxopts 解析 ---
        // 伪造一个 (argc, char**) 列表
        std::vector<char*> c_argv;
        std::string program_name = "task-cli"; // 伪造的 argv[0]
        c_argv.push_back(const_cast<char*>(program_name.c_str()));

        for (const auto& s : args) {
            c_argv.push_back(const_cast<char*>(s.c_str()));
        }
        int c_argc = static_cast<int>(c_argv.size());

        cxxopts::ParseResult result;
        try {
            // 解析
            result = options.parse(c_argc, c_argv.data());
        }
        catch (const cxxopts::exceptions::exception& e) {
            std::cerr << "错误: " << e.what() << std::endl;
            std::cerr << "输入 'help' 查看可用选项。" << std::endl;
            continue; // 继续 REPL 循环, 不退出
        }

        // 7. 逻辑分发 (在 REPL 循环内)
        try {
            // (注意：'help' 现在被 REPL 循环处理了，但以防万一 --help 传入)
            if (result.count("help")) {
                std::cout << options.help() << std::endl;
            }

            // --- 增 (Add) ---
            else if (result.count("add")) {
                std::string description = result["add"].as<std::string>();
                Task* new_task = manager.add_task(description);
                print_added_task(new_task);
            }

            // --- 删 (Remove) ---
            else if (result.count("remove")) {
                int id = result["remove"].as<int>();
                bool success = manager.remove_task(id);
                print_removed_task(id, success);
            }

            // --- 查 (Get) ---
            else if (result.count("get")) {
                int id = result["get"].as<int>();
                Task* task = manager.get_task(id);
                print_get_task(task);
            }

            // --- 改 (Update) ---
            else if (result.count("update")) {
                int id = result["update"].as<int>();
                bool updated = false;
                Task* task = nullptr;

                if (result.count("desc")) {
                    std::string description = result["desc"].as<std::string>();
                    task = manager.update_task_description(id, description);
                    updated = (task != nullptr);
                }
                if (result.count("status")) {
                    std::string status_str = result["status"].as<std::string>();
                    try {
                        string_to_status(status_str); // 验证输入
                        task = manager.update_task_status(id, status_str);
                        updated = (task != nullptr);
                    }
                    catch (const std::invalid_argument&) {
                        std::cerr << "错误: 无效的状态值 '" << status_str << "'。请使用 TO_DO, IN_PROGRESS, 或 DONE。" << std::endl;
                    }
                }

                if (!updated && !result.count("desc") && !result.count("status")) {
                    std::cerr << "错误: 使用 --update 必须至少提供 --desc 或 --status。" << std::endl;
                }
                else if (task) {
                    print_updated_task(task);
                }
                else if (updated) {
                    // task 可能是 nullptr 但 updated 是 true (e.g. 连续两次更新)
                    // 重新获取任务来打印
                    print_updated_task(manager.get_task(id));
                }
            }

            // --- 查 (List - 默认) ---
            // 如果没有提供任何其他匹配的命令，则默认为 list
            else if (result.count("list") || result.arguments().empty()) {
                std::vector<const Task*> tasks;
                if (result.count("status")) {
                    std::string status_str = result["status"].as<std::string>();
                    try {
                        TaskStatus statu = string_to_status(status_str);
                        tasks = manager.list_tasks(statu);
                    }
                    catch (const std::invalid_argument&) {
                        std::cerr << "错误: 无效的状态值 '" << status_str << "'。请使用 TO_DO, IN_PROGRESS, 或 DONE。" << std::endl;
                        continue; // 跳过打印
                    }
                }
                else {
                    tasks = manager.list_tasks();
                }
                print_tasks(tasks);
            }
            else {
                    std::cerr << "未知命令。输入 'help' 查看可用选项。" << std::endl;
			}

        }
        catch (const std::exception& e) {
            std::cerr << "程序运行时发生错误: " << e.what() << std::endl;
            // 继续 REPL 循环
        }
    } // 结束 while(true)

    std::cout << "正在退出..." << std::endl;
    return 0;
}