#pragma once

#include <iostream>
#include <vector>
#include "task.h"

void print_task(const Task* task);
void print_tasks(const std::vector<const Task*>& tasks); 
void print_added_task(const Task* task);
void print_get_task(const Task* task);
void print_removed_task(const int& id, bool success);
void print_updated_task(const Task* task);