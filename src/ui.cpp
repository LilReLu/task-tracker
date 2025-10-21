#pragma once

#include <iostream>
#include <vector>
#include "task.h"
#include "ui.h"

void print_task(const Task* task) {
	if (task == nullptr) {
		std::cout << "Task is null." << std::endl;
		return;
	}
	std::cout << "-------------------------" << std::endl;
	std::cout << "Task ID: " << task->get_id() << std::endl;
	std::cout << "Description: " << task->get_description() << std::endl;
	std::cout << "Status: " << status_to_string(task->get_status()) << std::endl;
	std::time_t created_at = task->get_created_at();
	std::time_t updated_at = task->get_updated_at();
	std::cout << "Created At: " << std::asctime(std::localtime(&created_at));
	std::cout << "Updated At: " << std::asctime(std::localtime(&updated_at));
	std::cout << "-------------------------" << std::endl;
}

void print_tasks(const std::vector<const Task*>& tasks) {
	if (tasks.empty()) {
		std::cout << "No tasks to display." << std::endl;
		return;
	}
	for (const auto& task : tasks) {
		print_task(task);
		std::cout << "------------------------" << std::endl;
	}
}