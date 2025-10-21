#include "task.h"

int main() {
	Task myTask("1", "Implement task management system");
	myTask.update_status("IN_PROGRESS");
	myTask.update_description("Implement task management system with C++");
	myTask.update_status("DONE");
	return 0;
}