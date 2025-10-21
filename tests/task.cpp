#include <gtest/gtest.h>
#include "task-tracker/task.h"
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