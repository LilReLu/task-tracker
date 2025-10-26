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

	Task* add_task(std::string description);
	bool remove_task(int id);
	bool remove_last_task();
	bool clear_all_tasks();
	Task* get_task(int id);

	Task* update_task_status(int id, std::string new_status);
	Task* update_task_description(int id, const std::string new_description);

	std::vector<const Task*> list_tasks();
	std::vector<const Task*> list_tasks(TaskStatus statu);

	bool IsEmpty() const {
		return tasks.empty();
	}

	int get_last_id() const {
		int last_id = 0;
		for (const auto& task:tasks){
			if (task->get_id() > last_id){
				last_id = task->get_id();
			}
		}
		return last_id;
	}

private:
	std::vector<std::unique_ptr<Task>> tasks;
	std::string filename;
	int next_id = 1;

	void ensure_file_exists(const std::string filename);
	void save_to_file();
	void load_from_file(std::string filename);
};

