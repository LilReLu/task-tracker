#include "task_manager.h"
#include <iostream>

TaskManager::TaskManager() : file_exists_flag(false), data_flag(false) {
	load_from_file("tasks.json");
}

void TaskManager::load_from_file(std::string filename) {
	std::ifstream file(filename);
}