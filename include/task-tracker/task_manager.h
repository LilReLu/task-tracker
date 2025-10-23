#include <vector>
#include <fstream>
#include <memory>
#include <ctime>
#include "task.h"
#include <nlohmann/json.hpp>
#pragma once

class TaskManager {

public:
	TaskManager(const std::string filename = "tasks.json");
	~TaskManager();

	TaskManager(const TaskManager&) = delete; // Disable copy constructor
	TaskManager& operator=(const TaskManager&) = delete; // Disable copy assignment

	Task* add_task(std::string id, std::string description);
	bool remove_task(std::string id);
	Task* get_task(std::string id);

	Task* update_task_status(std::string id, std::string new_status);
	Task* update_task_description(const std::string id, const std::string new_description);

	std::vector<const Task*> list_tasks();
	std::vector<const Task*> list_tasks(TaskStatus statu);

	bool IsEmpty() const {
		return tasks.empty();
	}

private:
	std::vector<std::unique_ptr<Task>> tasks;
	std::string filename;

	void ensure_file_exists(const std::string filename);
	void save_to_file();
	void load_from_file(std::string filename);
};

