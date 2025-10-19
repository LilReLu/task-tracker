#include "task_manager.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

void TaskManager::ensure_file_exists(const std::string filename) {
	std::ifstream test(filename);
	if( !test.good() ) {
		std::ofstream create_file(filename);
		create_file << "[]" ; // Initialize with empty JSON array
		create_file.close();
	}
}

void TaskManager::load_from_file(std::string filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return;
	}
	nlohmann::json j;
	file >> j;
	for (const auto& item : j) {
		std::string id = item["id"];
		std::string description = item["description"];
		std::unique_ptr<Task> task = std::make_unique<Task>(id, description);
		std::string status_str = item["status"];
		task->update_status(status_str);
		tasks.push_back(std::move(task));
	}
}

TaskManager::TaskManager() : data_flag(false) {
	ensure_file_exists("tasks.json");
	load_from_file("tasks.json");
}
TaskManager::~TaskManager() {
	/*if (data_flag) {
		save_to_file("tasks.json");
	}*/
}

void TaskManager::add_task(std::string id, std::string description) {
	std::unique_ptr<Task> task = std::make_unique<Task>(id, description);
	tasks.push_back(std::move(task));
	data_flag = true;
}

Task* TaskManager::get_task(std::string id) {
	for (const auto& task : tasks) {
		if (task->get_id() == id) {
			return task.get();
		}
	}
	return nullptr;
}