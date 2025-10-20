#include <gtest/gtest.h>
#include "task-tracker/task.h"
#include "task-tracker/task_manager.h"
#include <thread>  // 用于 std::this_thread::sleep_for
#include <chrono>  // 用于 std::chrono::seconds

// 测试构造函数是否正确设置了初始状态
TEST(TaskTest, ConstructorInitialization) {
    std::string id = "T-001";
    std::string desc = "Implement the core feature";

    Task task(id, desc);

    // 检查初始值
    EXPECT_EQ(task.get_id(), id);
    EXPECT_EQ(task.get_description(), desc);
    EXPECT_EQ(task.get_status(), TaskStatus::TO_DO); // 默认状态应为 TO_DO

    // 时间戳应该被设置
    EXPECT_NE(task.get_created_at(), 0);
    // 刚创建时，updated_at 应等于 created_at
    EXPECT_EQ(task.get_created_at(), task.get_updated_at());
}

TEST(TaskTest, UpdateStatus) {
    Task task("T-002", "Write unit tests");
    // 初始状态应为 TO_DO
    EXPECT_EQ(task.get_status(), TaskStatus::TO_DO);
    // 更新状态为 IN_PROGRESS
    task.update_status("IN_PROGRESS");
    EXPECT_EQ(task.get_status(), TaskStatus::IN_PROGRESS);
    // 更新状态为 DONE
    task.update_status("DONE");
    EXPECT_EQ(task.get_status(), TaskStatus::DONE);
    // 尝试设置无效状态，应保持原状态不变
    task.update_status("INVALID_STATUS");
    EXPECT_EQ(task.get_status(), TaskStatus::DONE);
}

TEST(TaskTest, UpdateDescription) {
    Task task("T-003", "Initial description");
	EXPECT_EQ(task.get_description(), "Initial description");
    std::string new_desc = "Updated description";
    // 更新描述
    task.update_description(new_desc);
    EXPECT_EQ(task.get_description(), new_desc);
}

TEST(TaskTest, TimeUpdateOnModification) {
    Task task("T-004", "Check timestamps");
    std::time_t created_at = task.get_created_at();
    std::time_t updated_at = task.get_updated_at();
    // 刚创建时，created_at 和 updated_at 应相等
    EXPECT_EQ(created_at, updated_at);
    // 等待一秒钟以确保时间戳会改变
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 更新描述，应该更新 updated_at
    task.update_description("New description");
    std::time_t new_updated_at = task.get_updated_at();
    EXPECT_GT(new_updated_at, updated_at); // updated_at 应该更新
    // 再次等待一秒钟
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // 更新状态，应该再次更新 updated_at
    task.update_status("IN_PROGRESS");
    std::time_t latest_updated_at = task.get_updated_at();
    EXPECT_GT(latest_updated_at, new_updated_at); // updated_at 应该再次更新
}

TEST(TaskTest, RemoveTask) {
    Task* task = new Task("T-005", "Task to be removed");
    // 确保任务已创建
    EXPECT_EQ(task->get_id(), "T-005");
    // 移除任务
    task->remove();
    // 由于 remove 调用了析构函数，task 指针现在悬空，不能再访问它
    // 这里我们只是确保没有崩溃发生
    SUCCEED();
}

TEST(TaskManagerTest, AddAndGetTask) {
    TaskManager manager;
	Task* empty_task = manager.get_task("NON_EXISTENT");
	EXPECT_EQ(empty_task, nullptr); // 确保不存在的任务返回 nullptr
    manager.add_task("T-006", "Manage tasks effectively");
    Task* task = manager.get_task("T-006");
    ASSERT_NE(task, nullptr); // 确保任务被正确添加
    EXPECT_EQ(task->get_id(), "T-006");
    EXPECT_EQ(task->get_description(), "Manage tasks effectively");
}

TEST(TaskManagerTest, RemoveTask) {
    TaskManager manager;
    manager.add_task("T-007", "Task to be removed from manager");
    Task* task = manager.get_task("T-007");
    ASSERT_NE(task, nullptr); // 确保任务被正确添加
    manager.remove_task("T-007");
    Task* removed_task = manager.get_task("T-007");
    EXPECT_EQ(removed_task, nullptr); // 确保任务被正确移除
}

TEST(TaskManagerTest, UpdateTaskStatusAndDescription) {
    TaskManager manager;
    manager.add_task("T-008", "Initial description");
    manager.update_task_status("T-008", "IN_PROGRESS");
    manager.update_task_description("T-008", "Updated description");
    Task* task = manager.get_task("T-008");
    ASSERT_NE(task, nullptr); // 确保任务存在
    EXPECT_NE(task->get_status(), TaskStatus::TO_DO);
    EXPECT_EQ(task->get_status(), TaskStatus::IN_PROGRESS);
    EXPECT_NE(task->get_description(), "Initial description");
    EXPECT_EQ(task->get_description(), "Updated description");
}

TEST(TaskManagerTest, ListAllTasks) {
    TaskManager manager;
    manager.add_task("T-001", "Task 1");
    manager.add_task("T-002", "Task 2");
    // 捕获标准输出
    testing::internal::CaptureStdout();
    manager.list_tasks();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-001"), std::string::npos); // 应包含 T-001
    EXPECT_NE(output.find("T-002"), std::string::npos); // 应包含 T-002
}

TEST(TaskManagerTest, ListTasksByStatus) {
    TaskManager manager;
    manager.add_task("T-009", "Task 1");
    manager.add_task("T-010", "Task 2");
    manager.update_task_status("T-010", "DONE");
    // 捕获标准输出
    testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::TO_DO);
    std::string output_todo = testing::internal::GetCapturedStdout();
    EXPECT_NE(output_todo.find("T-009"), std::string::npos); // 应包含 T-009
    EXPECT_EQ(output_todo.find("T-010"), std::string::npos); // 不应包含 T-010
    testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::DONE);
    std::string output_done = testing::internal::GetCapturedStdout();
    EXPECT_NE(output_done.find("T-010"), std::string::npos); // 应包含 T-010
    EXPECT_EQ(output_done.find("T-009"), std::string::npos); // 不应包含 T-009
	testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::IN_PROGRESS);
    std::string output_in_progress = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_in_progress.find("T-009"), std::string::npos); // 不应包含 T-009
	EXPECT_EQ(output_in_progress.find("T-010"), std::string::npos); // 不应包含 T-010
}

