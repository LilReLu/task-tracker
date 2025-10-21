#include <gtest/gtest.h>
#include "task-tracker/task.h"
#include "task-tracker/task_manager.h"
#include <thread>  // 用于 std::this_thread::sleep_for
#include <chrono>  // 用于 std::chrono::seconds

class FilereaderTest : public ::testing::Test {
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
    manager.add_task("T-0011", "Task 11");
    manager.add_task("T-0012", "Task 12");
    // 捕获标准输出
    testing::internal::CaptureStdout();
    manager.list_tasks();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-0011"), std::string::npos); // 应包含 T-0011
    EXPECT_NE(output.find("T-0012"), std::string::npos); // 应包含 T-0012
}

TEST_F(FilereaderTest, ListTasksByStatu) {
    TaskManager manager("test_tasks.json");
    // 捕获标准输出
    testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::TO_DO);
    std::string output = testing::internal::GetCapturedStdout();
    // 应包含 T-001 和 T-004
    EXPECT_NE(output.find("T-001"), std::string::npos);
    EXPECT_NE(output.find("T-004"), std::string::npos);
    // 不应包含 T-002 和 T-003
    EXPECT_EQ(output.find("T-002"), std::string::npos);
	EXPECT_EQ(output.find("T-003"), std::string::npos);

	testing::internal::CaptureStdout();
	manager.list_tasks(TaskStatus::IN_PROGRESS);
    output = testing::internal::GetCapturedStdout();
    // 应包含 T-002
    EXPECT_NE(output.find("T-002"), std::string::npos);
    // 不应包含 T-001、T-003 和 T-004
    EXPECT_EQ(output.find("T-001"), std::string::npos);
    EXPECT_EQ(output.find("T-003"), std::string::npos);
	EXPECT_EQ(output.find("T-004"), std::string::npos);

	testing::internal::CaptureStdout();
	manager.list_tasks(TaskStatus::DONE);
    output = testing::internal::GetCapturedStdout();
    // 应包含 T-003
    EXPECT_NE(output.find("T-003"), std::string::npos);
    // 不应包含 T-001、T-002 和 T-004
    EXPECT_EQ(output.find("T-001"), std::string::npos);
    EXPECT_EQ(output.find("T-002"), std::string::npos);
	EXPECT_EQ(output.find("T-004"), std::string::npos);
}

TEST_F(FilereaderTest, LoadFromFile) {
    TaskManager manager("test_tasks.json");
    // 检查任务是否正确加载
    Task* task1 = manager.get_task("T-001");
    ASSERT_NE(task1, nullptr);
    EXPECT_EQ(task1->get_description(), "Test task 1");
    EXPECT_EQ(task1->get_status(), TaskStatus::TO_DO);
	EXPECT_EQ(task1->get_created_at(), 1672567200);
	EXPECT_EQ(task1->get_updated_at(), 1672567200);
    Task* task2 = manager.get_task("T-002");
    ASSERT_NE(task2, nullptr);
    EXPECT_EQ(task2->get_description(), "Test task 2");
    EXPECT_EQ(task2->get_status(), TaskStatus::IN_PROGRESS);
	EXPECT_EQ(task2->get_created_at(), 1672657200);
	EXPECT_EQ(task2->get_updated_at(), 1672658200);
    Task* task3 = manager.get_task("T-003");
    ASSERT_NE(task3, nullptr);
    EXPECT_EQ(task3->get_description(), "Test task 3");
    EXPECT_EQ(task3->get_status(), TaskStatus::DONE);
    Task* task4 = manager.get_task("T-004");
    ASSERT_NE(task4, nullptr);
    EXPECT_EQ(task4->get_description(), "Test task 4");
    EXPECT_EQ(task4->get_status(), TaskStatus::TO_DO);
}

TEST_F(FilereaderTest, readEMptyFile) {
    std::string empty_filename = "empty_test_file.json";
	// 创建一个空文件
	std::ofstream ofs(empty_filename);
    ASSERT_TRUE(ofs.is_open());
    ofs.close(); // 立即关闭，使其为空

	TaskManager manager(empty_filename);
    // 确保没有任务被加载
	EXPECT_EQ(manager.IsEmpty(), true);
    // 删除测试文件
	std::remove(empty_filename.c_str());
}
TEST_F(FilereaderTest, FileCreationIfNotExist) {
    std::string non_existent_file = "non_existent_test_file.json";
    // 确保文件不存在
    std::remove(non_existent_file.c_str());
    // 创建 TaskManager 实例，应该会创建文件
    TaskManager manager(non_existent_file);
    // 检查文件是否被创建    
    std::ifstream ifs(non_existent_file);
    EXPECT_TRUE(ifs.is_open()) << "File was not created.";
    ifs.close();
    // 删除测试文件
    std::remove(non_existent_file.c_str());
}

TEST_F(FilereaderTest, SaveToFile) {
    TaskManager manager("test_tasks.json");
    // 添加一个新任务
    manager.add_task("T-005", "Newly added task");
    // 保存到文件
    manager.update_task_status("T-005", "IN_PROGRESS");
    manager.update_task_description("T-005", "Updated description for new task");
    // 重新加载文件以验证保存
    TaskManager reloaded_manager("test_tasks.json");
    Task* task = reloaded_manager.get_task("T-005");
    ASSERT_NE(task, nullptr); // 确保任务存在
    EXPECT_EQ(task->get_description(), "Updated description for new task");
    EXPECT_EQ(task->get_status(), TaskStatus::IN_PROGRESS);
}
