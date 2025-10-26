#include "task_manager.h"
#include <memory>
#include <stdexcept> // 用于抛出异常
#include <iostream>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>

TaskManager::TaskManager(const std::string filename )
	: filename(filename) {
	ensure_file_exists(filename);
	load_from_file(filename);
}
TaskManager::~TaskManager() {
	/*if (data_flag) {
		save_to_file("tasks.json");
	}*/
}

void TaskManager::ensure_file_exists(const std::string filename) {
	std::ifstream test(filename);
	if( !test.good() ) {
		std::ofstream create_file(filename);
		create_file << "[]" ; // Initialize with empty JSON array
		create_file.close();

		std::cerr << "File " << filename << " did not exist and was created." << std::endl;
	}
}

void TaskManager::load_from_file(std::string filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		throw std::runtime_error("Could not open file: " + filename);
		return;
	}

	file.seekg(0, std::ios::end);
	if (file.tellg() == 0) {
		// 文件是空的 (0 字节)，直接返回
		file.close();
		return;
	}

	file.seekg(0, std::ios::beg); // 重置文件指针到开头

	nlohmann::json j;
	try {
		file >> j;
		file.close();

		this->tasks.clear(); // 清空现有任务列表
		this->next_id = 1; // 重置 next_id

		if (j.contains("next_id")) {
			next_id = j["next_id"].get<int>();
		};

		if(j.contains("tasks")) {
			for (const auto& item : j["tasks"]) {
				int id = item["id"];
				std::string description = item["description"];
				std::time_t created_at = item.value("created_at", std::time_t());
				std::time_t updated_at = item.value("updated_at", std::time_t());
				std::string status_str = item["status"];
				TaskStatus statu = string_to_status(status_str);
				std::unique_ptr<Task> task = std::make_unique<Task>(id, description, statu, created_at, updated_at);
				tasks.push_back(std::move(task));
			}
		}
	}catch (const nlohmann::json::parse_error& e) {
		std::cerr << "JSON parse error in file " << filename << ": " << e.what() << std::endl;
	}
}
	
	


Task* TaskManager::add_task(std::string description) {
	std::unique_ptr<Task> task = std::make_unique<Task>(next_id, description);
	tasks.push_back(std::move(task));
	next_id++;
	save_to_file();
	return tasks.back().get();
}

bool TaskManager::remove_task(int id) {
	const size_t original_size = tasks.size();

	tasks.erase(
		std::remove_if(tasks.begin(), tasks.end(),
			[&id](const std::unique_ptr<Task>& task) {
				return task->get_id() == id;
			}),
		tasks.end()
	);
	const size_t new_size = tasks.size();
	save_to_file();
	return new_size < original_size;
}

bool TaskManager::remove_last_task() {
	if (tasks.empty()) {
		return false; // No tasks to remove
	}
	tasks.pop_back();
	save_to_file();
	return true;
}

bool TaskManager::clear_all_tasks() {
	if (tasks.empty()) {
		return false; // No tasks to clear
	}
	tasks.clear();
	save_to_file();
	return true;
}

Task* TaskManager::get_task(int id) {
	for (const auto& task : tasks) {
		if (task->get_id() == id) {
			return task.get();
		}
	}
	return nullptr;
}

Task*
TaskManager::update_task_status(int id, std::string new_status) {
	Task* task = get_task(id);
	if (task) {
		task->update_status(new_status);
	}
	else {
		std::cerr << "Task with ID " << id << " not found." << std::endl;
	}
	save_to_file();
	return task;
}

Task* TaskManager::update_task_description(int id, const std::string new_description) {
	Task* task = get_task(id);
	if (task) {
		task->update_description(new_description);
	}
	else {
		std::cerr << "Task with ID " << id << " not found." << std::endl;
	}
	save_to_file();
	return task;
}

std::vector<const Task*> TaskManager::list_tasks() {
	std::vector<const Task*> task_list;
	tasks.reserve(tasks.size());
	for (const auto& task : tasks) {
		task_list.push_back(task.get());
	}
	return task_list;
}

std::vector<const Task*> TaskManager::list_tasks(TaskStatus statu){
	std::vector<const Task*> filtered_tasks;
	for (const auto& task : tasks) {
		if( task->get_status() == statu ) {
			filtered_tasks.push_back( task.get() );
		}
	}
	return filtered_tasks;
}



void TaskManager::save_to_file() {
	nlohmann::json j_root;

	j_root["next_id"] = next_id; 

	nlohmann::json j_tasks = nlohmann::json::array();

	for (const auto& task : tasks) {
		nlohmann::json j_task;

		if (!task) {
			continue; // Skip null tasks
		}

		j_task["id"] = task->get_id();
		j_task["description"] = task->get_description();
		j_task["status"] = status_to_string(task->get_status());
		j_task["created_at"] = task->get_created_at();
		j_task["updated_at"] = task->get_updated_at();
		j_tasks.push_back(j_task);
	}
	j_root["tasks"] = j_tasks;

	std::ofstream file(filename);
	if(!file.is_open()) {
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		throw std::runtime_error("Could not open file for writing: " + filename);
		return;
	}

	file << j_root.dump(4); // Pretty print with 4 spaces indentation
	file.close();
}