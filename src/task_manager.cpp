#include "task_manager.h"
#include <memory>
#include <stdexcept> // �����׳��쳣
#include <iostream>
#include <fstream>
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
	nlohmann::json j;
	try {
		file >> j;
		for (const auto& item : j) {
			std::string id = item["id"];
			std::string description = item["description"];
			std::time_t created_at = item.value("created_at", std::time_t());
			std::time_t updated_at = item.value("updated_at", std::time_t());
			std::string status_str = item["status"];
			TaskStatus statu = string_to_status(status_str);
			std::unique_ptr<Task> task = std::make_unique<Task>(id, description, statu, created_at, updated_at);
			tasks.push_back(std::move(task));
		}
	}
	catch (const std::exception& e) {
		// ����ļ��Ƿ����Ϊ��
		file.seekg(0, std::ios::end); // �ƶ����ļ�ĩβ
		if (file.tellg() == 0) {
			// �ļ��ǿյ� (0 �ֽ�)
			// ���Ǿ�����"���ļ�"��Ϊ�յ������б�
			// ��Ҫ�׳��쳣�������ط���
		}
		else {
			// �ļ����ǿյģ��� JSON ��ʽ���ˣ�����һ�������Ĵ���
			throw; // �����׳��쳣
		}
	}
}

Task* TaskManager::add_task(std::string id, std::string description) {
	std::unique_ptr<Task> task = std::make_unique<Task>(id, description);
	tasks.push_back(std::move(task));
	save_to_file();
	return tasks.back().get();
}

bool TaskManager::remove_task(std::string id) {
	const size_t original_size = tasks.size();

	tasks.erase(
		std::remove_if(tasks.begin(), tasks.end(),
			[&id](const std::unique_ptr<Task>& task) {
				return task->get_id() == id;
			}),
		tasks.end()
	);
	const size_t new_size = tasks.size();
	return new_size < original_size;
}

Task* TaskManager::get_task(std::string id) {
	for (const auto& task : tasks) {
		if (task->get_id() == id) {
			return task.get();
		}
	}
	return nullptr;
}

Task*
TaskManager::update_task_status(std::string id, std::string new_status) {
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

Task* TaskManager::update_task_description(const std::string id, const std::string new_description) {
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
	nlohmann::json j = nlohmann::json::array();
	for (const auto& task : tasks) {
		nlohmann::json item;
		item["id"] =  task->get_id();
		item["description"] = task->get_description();
		item["status"] = status_to_string( task->get_status() );
		item["created_at"] = task->get_created_at();
		item["updated_at"] = task->get_updated_at();
		j.push_back(item);
	}
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}
	file << j.dump(4); // Pretty print with 4 spaces indentation
}