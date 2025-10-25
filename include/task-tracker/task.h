#pragma once
#include <string>
#include <ctime>
#include <stdexcept>

enum class TaskStatus {
	TO_DO,
	IN_PROGRESS,
	DONE
};

// 将字符串转换为TaskStatus枚举值
inline TaskStatus string_to_status(const std::string& s) {
	if (s == "IN_PROGRESS") return TaskStatus::IN_PROGRESS;
	if (s == "DONE") return TaskStatus::DONE;
	if (s == "TO_DO") return TaskStatus::TO_DO;
	throw std::invalid_argument("Invalid status string");
}

// 将TaskStatus枚举值转换为字符串
inline std::string status_to_string(TaskStatus status) {
	switch (status) {
	case TaskStatus::TO_DO: return "TO_DO";
	case TaskStatus::IN_PROGRESS: return "IN_PROGRESS";
	case TaskStatus::DONE: return "DONE";
	default: return "UNKNOWN";
	}
}

class Task {

public:
	Task(int id, const std::string description);
	Task(int id, const std::string description, TaskStatus statu, std::time_t creat, std::time_t update);
	~Task();

	Task(const Task&) = delete; // Disable copy constructor
	Task& operator=(const Task&) = delete; // Disable copy assignment


	void update_status(const std::string); // Update task status
	void update_description(const std::string); // Update task description

	// --- Getters (为测试添加) ---
	const int& get_id() const { return id; }
	const std::string& get_description() const { return description; }
	TaskStatus get_status() const { return status; }
	std::time_t get_created_at() const { return created_at; }
	std::time_t get_updated_at() const { return updated_at; }

private:
	int id;
	std::string description;
	TaskStatus status;

	std::time_t created_at;
	std::time_t updated_at;
};