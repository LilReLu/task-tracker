#include <vector>
#include <fstream>
#include "task.h"
#include <nlohmann/json.hpp>

class TaskManager {

public:
	TaskManager();
	~TaskManager();

	TaskManager(const TaskManager&) = delete; // Disable copy constructor
	TaskManager& operator=(const TaskManager&) = delete; // Disable copy assignment

	void add_task(std::string id, std::string description);
	void remove_task(std::string id);

	void update_task_status(std::string id, std::string new_status);
	void update_task_description(std::string id, std::string new_description);

	Task* get_task(std::string id);
	void list_tasks();

private:
	std::vector<Task*> tasks;
	bool file_exists_flag;
	bool data_flag;

	void save_to_file(std::string filename);
	void load_from_file(std::string filename);
};