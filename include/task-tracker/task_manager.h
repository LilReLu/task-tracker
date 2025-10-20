#include <vector>
#include <fstream>
#include <memory>
#include "task.h"
#include <nlohmann/json.hpp>
#pragma once

class TaskManager {

public:
	TaskManager();
	~TaskManager();

	TaskManager(const TaskManager&) = delete; // Disable copy constructor
	TaskManager& operator=(const TaskManager&) = delete; // Disable copy assignment

	void add_task(std::string id, std::string description);
	void remove_task(std::string id);
	Task* get_task(std::string id);

	void update_task_status(std::string id, std::string new_status);
	void update_task_description(const std::string id,const std::string new_description);

	void list_tasks();
	void list_tasks(TaskStatus statu);

private:
	std::vector<std::unique_ptr<Task>> tasks;
	bool data_flag;

	void ensure_file_exists(const std::string filename);
	void save_to_file(std::string filename);
	void load_from_file(std::string filename);
};