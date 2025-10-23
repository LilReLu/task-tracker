#include "cxxopts.hpp"
#include "task_manager.h"
#include "ui.h"
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    TaskManager manager("tasks.json");
    std::string line;

    std::cout << "Task CLI (cxxopts版). 输入 help 查看命令，输入 exit 退出。\n";
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::vector<std::string> args;
        std::string arg;
        while (iss >> std::quoted(arg) || iss >> arg) {
            args.push_back(arg);
        }
        if (args.empty()) continue;

        if (args[0] == "exit" || args[0] == "quit") {
            break;
        }
        if (args[0] == "help") {
            std::cout << "命令:\n"
                << "  list\n"
                << "  add --id <id> --description <desc>\n"
                << "  remove --id <id>\n"
                << "  get --id <id>\n"
                << "  update --id <id> --description <desc>\n"
                << "  exit\n";
            continue;
        }

        // 构造 argc/argv 以便 cxxopts 解析
        std::vector<const char*> argv;
        argv.push_back("task"); // 程序名
        for (const auto& s : args) argv.push_back(s.c_str());
        int argc = static_cast<int>(argv.size());

        try {
            cxxopts::Options options("task", "Task management CLI");
            options.add_options()
                ("id", "Task ID", cxxopts::value<std::string>()->default_value(""))
                ("description", "Task description", cxxopts::value<std::string>()->default_value(""))
                ("h,help", "Print usage");

            auto result = options.parse(argc, argv.data());

            std::string cmd = args[0];
            if (cmd == "list") {
                auto tasks = manager.list_tasks();
                print_tasks(tasks);
            }
            else if (cmd == "add") {
                auto task = manager.add_task(result["id"].as<std::string>(), result["description"].as<std::string>());
                print_added_task(task);
            }
            else if (cmd == "remove") {
                bool success = manager.remove_task(result["id"].as<std::string>());
                print_removed_task(result["id"].as<std::string>(), success);
            }
            else if (cmd == "get") {
                auto task = manager.get_task(result["id"].as<std::string>());
                print_get_task(task);
            }
            else if (cmd == "update") {
                auto task = manager.update_task_description(result["id"].as<std::string>(), result["description"].as<std::string>());
                print_updated_task(task);
            }
            else {
                std::cout << "未知命令: " << cmd << "\n";
            }
        }
        catch (const cxxopts::exceptions::exception& e) {
            std::cout << "参数错误: " << e.what() << "\n";
        }
    }
    return 0;
}