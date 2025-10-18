#pragma
#include <string>
#include <ctime>
#include <stdexcept>

enum class TaskStatus {
	TO_DO,
	IN_PROGRESS,
	DONE
};

// 辅助函数，用于将字符串转换为枚举
inline TaskStatus string_to_status(const std::string& s) {
	if (s == "IN_PROGRESS") return TaskStatus::IN_PROGRESS;
	if (s == "DONE") return TaskStatus::DONE;
	if (s == "TO_DO") return TaskStatus::TO_DO;
	throw std::invalid_argument("Invalid status string");
}

class Task {

public:
	Task(std::string id, std::string description);
	~Task();

	Task(const Task&) = delete; // Disable copy constructor
	Task& operator=(const Task&) = delete; // Disable copy assignment


	void update_status(std::string); // Update task status
	void update_description(std::string); // Update task description
	void remove(); // Remove task

	// --- Getters (为测试添加) ---
	const std::string& get_id() const { return id; }
	const std::string& get_description() const { return description; }
	TaskStatus get_status() const { return status; }
	std::time_t get_created_at() const { return created_at; }
	std::time_t get_updated_at() const { return updated_at; }

private:
	std::string id;
	std::string description;
	TaskStatus status;

	std::time_t created_at;
	std::time_t updated_at;
};