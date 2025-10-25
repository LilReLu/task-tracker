#include "cxxopts.hpp"
#include "task_manager.h"
#include "ui.h"
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    TaskManager manager("tasks.json");
    std::string line;

    std::cout << "Task CLI (cxxopts°æ). ÊäÈë help ²é¿´ÃüÁî£¬ÊäÈë exit ÍË³ö¡£\n";
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
            std::cout << "ÃüÁî:\n"
                << "  list\n"
                << "  add --id <id> --description <desc>\n"
                << "  remove --id <id>\n"
                << "  get --id <id>\n"
                << "  update --id <id> --description <desc>\n"
                << "  exit\n";
            continue;
        }
    }
}