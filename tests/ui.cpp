#include <gtest/gtest.h>
#include "ui.h"
#include "task_manager.h"

class PrintTest : public ::testing::Test {
protected:
    std::string test_file = "test_tasks.json";
    std::string test_json_content = R"([
        {
            "id": "T-001",
            "description": "Test task 1",
            "status": "TO_DO",
            "created_at": 1672567200,
            "updated_at": 1672567200
        },
        {
            "id": "T-002",
            "description": "Test task 2",
            "status": "IN_PROGRESS",
            "created_at": 1672657200,
            "updated_at": 1672658200
        },
        {
            "id": "T-003",
            "description": "Test task 3",
            "status": "DONE",
            "created_at": 1672743600,
            "updated_at": 1672744600
        },
        {
            "id": "T-004",
            "description": "Test task 4",
            "status": "TO_DO",
            "created_at": 1672830000,
            "updated_at": 1672830000
        },
        {
            "id": "T-005",
            "description": "Test task 5",
            "status": "TO_DO",
            "created_at": 1672830099,
            "updated_at": 1672830059
        }
    ])";
    void SetUp() override {
        // 在每个测试前创建测试文件
        std::ofstream ofs(test_file);
        ASSERT_TRUE(ofs.is_open()) << "Failed to create test file for SetUp.";

        ofs << test_json_content;
        ofs.close();
    }
    void TearDown() override {
        // 在每个测试后删除测试文件
        std::remove(test_file.c_str());
    }
};

TEST_F(PrintTest, PrintTests) {
    TaskManager manager(test_file);
    std::vector<const Task*> all_tasks = manager.list_tasks();
    testing::internal::CaptureStdout();
    print_tasks(all_tasks);
    std::string output = testing::internal::GetCapturedStdout();
    // 检查输出是否包含所有任务的 ID ,状态和描述
    for (const auto& task : all_tasks) {
        EXPECT_NE(output.find(task->get_id()), std::string::npos);
        EXPECT_NE(output.find(task->get_description()), std::string::npos);
    }
}

TEST_F(PrintTest, PrintSingleTask) {
    TaskManager manager(test_file);
    Task* task = manager.get_task("T-002");
    testing::internal::CaptureStdout();
    print_task(task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-002"), std::string::npos);
    EXPECT_NE(output.find("Test task 2"), std::string::npos);
    EXPECT_NE(output.find("IN_PROGRESS"), std::string::npos);
}

TEST_F(PrintTest, PrintAddedTask) {
    TaskManager manager(test_file);
    Task* new_task = manager.add_task("T-006", "Newly added task");
    testing::internal::CaptureStdout();
    print_added_task(new_task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-006"), std::string::npos);
    EXPECT_NE(output.find("Newly added task"), std::string::npos);
}

TEST_F(PrintTest, PrintRemovedTask) {
    TaskManager manager(test_file);
    bool success = manager.remove_task("T-003");
    testing::internal::CaptureStdout();
    print_removed_task("T-003", success);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("removed successfully"), std::string::npos);
    // 尝试移除不存在的任务
    success = manager.remove_task("NON_EXISTENT");
    testing::internal::CaptureStdout();
    print_removed_task("NON_EXISTENT", success);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("not found"), std::string::npos);
}

TEST_F(PrintTest, PrintUpdatedTask) {
    TaskManager manager(test_file);
    Task* task = manager.update_task_description("T-004", "Updated description for task 4");
    testing::internal::CaptureStdout();
    print_updated_task(task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-004"), std::string::npos);
    EXPECT_NE(output.find("Updated description for task 4"), std::string::npos);
}

TEST_F(PrintTest, PrintGetTask) {
    TaskManager manager(test_file);
    Task* task = manager.get_task("T-001");
    testing::internal::CaptureStdout();
    print_get_task(task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-001"), std::string::npos);
    EXPECT_NE(output.find("Test task 1"), std::string::npos);
    // 尝试获取不存在的任务
    task = manager.get_task("NON_EXISTENT");
    testing::internal::CaptureStdout();
    print_get_task(task);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Task not found"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}