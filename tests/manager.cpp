#include <gtest/gtest.h>
#include "task-tracker/task_manager.h"
#include <thread>  // 用于 std::this_thread::sleep_for
#include <chrono>  // 用于 std::chrono::seconds

class EmptyManagerTest : public ::testing::Test {
protected:
    // 为这个 Fixture 定义一个唯一的、临时的文件名
    std::string test_file_name = "empty_manager_test_file.json";

    void SetUp() override {
        // 在 *每个* TEST_F 之前运行
        // 确保文件是干净的，只包含一个空的 JSON 数组
        std::ofstream ofs(test_file_name);
        ASSERT_TRUE(ofs.is_open());
        ofs << "[]"; // 写入一个空的 JSON 数组
        ofs.close();
    }

    void TearDown() override {
        // 在 *每个* TEST_F 之后运行
        // 删除这个临时文件，为下一个测试做准备
        std::remove(test_file_name.c_str());
    }
};

class FilereaderTest : public ::testing::Test {
protected:
    std::string test_file = "test_tasks.json";
    std::string test_json_content = R"({
    "next_id": 6,
    "tasks": [
        {
            "id": 1,
            "description": "Test task 1",
            "status": "TO_DO",
            "created_at": 1672567200,
            "updated_at": 1672567200
        },
        {
            "id": 2,
            "description": "Test task 2",
            "status": "IN_PROGRESS",
            "created_at": 1672657200,
            "updated_at": 1672658200
        },
        {
            "id": 3,
            "description": "Test task 3",
            "status": "DONE",
            "created_at": 1672743600,
            "updated_at": 1672744600
        },
        {
            "id": 4,
            "description": "Test task 4",
            "status": "TO_DO",
            "created_at": 1672830000,
            "updated_at": 1672830000
        },
        {
            "id": 5,
            "description": "Test task 5",
            "status": "TO_DO",
            "created_at": 1672830099,
            "updated_at": 1672830059
        }
    ]
})"; // 注意这里的括号
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

TEST_F(EmptyManagerTest, AddAndGetTask) {
    TaskManager manager(test_file_name);
	Task* empty_task = manager.get_task(648);
	EXPECT_EQ(empty_task, nullptr); // 确保不存在的任务返回 nullptr
    manager.add_task("Manage tasks effectively");
    Task* task = manager.get_task(1);
    ASSERT_NE(task, nullptr); // 确保任务被正确添加
    EXPECT_EQ(task->get_id(), 1);
    EXPECT_EQ(task->get_description(), "Manage tasks effectively");
}

TEST(TaskManagerTest, RemoveTask) {
    TaskManager manager;
    manager.add_task("Task to be removed from manager");
    Task* task = manager.get_task(1);
    ASSERT_NE(task, nullptr); // 确保任务被正确添加
    bool flag = manager.remove_task(1);
	EXPECT_EQ(flag, true); // 确保移除操作成功
    Task* removed_task = manager.get_task(1);
    EXPECT_EQ(removed_task, nullptr); // 确保任务被正确移除
}

TEST(TaskManagerTest, UpdateTaskStatusAndDescription) {
    TaskManager manager;
    manager.add_task("Initial description");
    manager.update_task_status(1, "IN_PROGRESS");
    manager.update_task_description(1, "Updated description");
    Task* task = manager.get_task(1);
    ASSERT_NE(task, nullptr); // 确保任务存在
    EXPECT_NE(task->get_status(), TaskStatus::TO_DO);
    EXPECT_EQ(task->get_status(), TaskStatus::IN_PROGRESS);
    EXPECT_NE(task->get_description(), "Initial description");
    EXPECT_EQ(task->get_description(), "Updated description");
}

TEST_F(EmptyManagerTest, ListAllTasks) {
    TaskManager manager(test_file_name);
    manager.add_task("Task 9");
    manager.add_task("Task 10");
    manager.add_task("Task 11");
	std::vector<const Task*> need_list = manager.list_tasks();
	EXPECT_EQ(need_list.size(), 3); // 确保返回了所有任务
}

TEST_F(EmptyManagerTest, ListTasksByStatu) {
	TaskManager manager(test_file_name);
	manager.add_task("Task 6");
	manager.add_task("Task 7");
	manager.add_task("Task 8");
	manager.add_task("Task 9");
	manager.add_task("Task 10");
	manager.add_task("Task 11");
	manager.update_task_status(4, "IN_PROGRESS");
	manager.update_task_status(5, "IN_PROGRESS");
	manager.update_task_status(6, "DONE");
    std::vector<const Task*> TO_DO_list = manager.list_tasks(TaskStatus::TO_DO);
	EXPECT_EQ(TO_DO_list.size(), 3); // 确保返回了所有 TO_DO 状态的任务
    std::vector<const Task*> IN_PROGRESS_list = manager.list_tasks(TaskStatus::IN_PROGRESS);
	EXPECT_EQ(IN_PROGRESS_list.size(), 2); // 确保返回了所有 IN_PROGRESS 状态的任务
    std::vector<const Task*> DONE_list = manager.list_tasks(TaskStatus::DONE);
	EXPECT_EQ(DONE_list.size(), 1); // 确保返回了所有 DONE 状态的任务
}

TEST_F(FilereaderTest, FileCreationIfNotExist) {
    std::string non_existent_file = "non_existent_test_file.json";
    // 确保文件不存在
    std::remove(non_existent_file.c_str());
    // 创建 TaskManager 实例，应该会创建文件
    TaskManager manager(non_existent_file);
    EXPECT_EQ(manager.IsEmpty(), true);
    // 检查文件是否被创建    
    std::ifstream ifs(non_existent_file);
    EXPECT_TRUE(ifs.is_open()) << "File was not created.";
    ifs.close();
    // 删除测试文件
    std::remove(non_existent_file.c_str());
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

TEST_F(FilereaderTest, LoadFromFile) {
    TaskManager manager("test_tasks.json");
    // 检查任务是否正确加载
    Task* task1 = manager.get_task(1);
    ASSERT_NE(task1, nullptr);
    EXPECT_EQ(task1->get_description(), "Test task 1");
    EXPECT_EQ(task1->get_status(), TaskStatus::TO_DO);
    EXPECT_EQ(task1->get_created_at(), 1672567200);
    EXPECT_EQ(task1->get_updated_at(), 1672567200);
    Task* task2 = manager.get_task(2);
    ASSERT_NE(task2, nullptr);
    EXPECT_EQ(task2->get_description(), "Test task 2");
    EXPECT_EQ(task2->get_status(), TaskStatus::IN_PROGRESS);
    EXPECT_EQ(task2->get_created_at(), 1672657200);
    EXPECT_EQ(task2->get_updated_at(), 1672658200);
    Task* task3 = manager.get_task(3);
    ASSERT_NE(task3, nullptr);
    EXPECT_EQ(task3->get_description(), "Test task 3");
    EXPECT_EQ(task3->get_status(), TaskStatus::DONE);
    Task* task4 = manager.get_task(4);
    ASSERT_NE(task4, nullptr);
    EXPECT_EQ(task4->get_description(), "Test task 4");
    EXPECT_EQ(task4->get_status(), TaskStatus::TO_DO);
}

TEST_F(FilereaderTest, SaveToFile) {
    TaskManager manager("test_tasks.json");
    std::vector<const Task*> list = manager.list_tasks();
    EXPECT_EQ(list.size(), 5); 
    // 添加一个新任务
    manager.add_task("Newly added task");
    std::vector<const Task*> new_list = manager.list_tasks();
	EXPECT_EQ(new_list.size(), 6);
    // 保存到文件
    manager.update_task_status(6, "IN_PROGRESS");
    manager.update_task_description(6, "Updated description for new task");
    // 重新加载文件以验证保存
    TaskManager reloaded_manager("test_tasks.json");
    std::vector<const Task*> final_list = reloaded_manager.list_tasks();
	EXPECT_EQ(final_list.size(), 6);
    Task* task = reloaded_manager.get_task(6);
    ASSERT_NE(task, nullptr); // 确保任务存在
    EXPECT_EQ(task->get_description(), "Updated description for new task");
    EXPECT_EQ(task->get_status(), TaskStatus::IN_PROGRESS);
}