#include "task.h"
#include <iostream>
#include <ctime>

Task::Task(const std::string id,const  std::string description) 
	: id(id), description(description), status(TaskStatus::TO_DO) {
		created_at = std::time(nullptr);
		updated_at = created_at;
}

Task::Task(const std::string id, const  std::string description, TaskStatus statu, std::time_t creat, std::time_t update)
	: id(id), description(description), status(statu), created_at(creat), updated_at(update) {
}

Task::~Task() {
	// Destructor logic if needed
}

void Task::update_status(const std::string new_status) {
	if (new_status == "TO_DO") {
		status = TaskStatus::TO_DO;
	} else if (new_status == "IN_PROGRESS") {
		status = TaskStatus::IN_PROGRESS;
	} else if (new_status == "DONE") {
		status = TaskStatus::DONE;
	} else {
		std::cerr << "Invalid status: " << new_status << std::endl;
		return;
	}
	updated_at = std::time(nullptr);
}

void Task::update_description(const std::string new_description) {
	description = new_description;
	updated_at = std::time(nullptr);
}



