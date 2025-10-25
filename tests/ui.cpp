#include <gtest/gtest.h>
#include "ui.h"
#include "task_manager.h"

class PrintTest : public ::testing::Test {
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
})"; // ע�����������
    void SetUp() override {
        // ��ÿ������ǰ���������ļ�
        std::ofstream ofs(test_file);
        ASSERT_TRUE(ofs.is_open()) << "Failed to create test file for SetUp.";

        ofs << test_json_content;
        ofs.close();
    }
    void TearDown() override {
        // ��ÿ�����Ժ�ɾ�������ļ�
        std::remove(test_file.c_str());
    }
};

TEST_F(PrintTest, PrintTests) {
    TaskManager manager(test_file);
    std::vector<const Task*> all_tasks = manager.list_tasks();
    testing::internal::CaptureStdout();
    print_tasks(all_tasks);
    std::string output = testing::internal::GetCapturedStdout();
    // �������Ƿ������������� ID ,״̬������
    for (const auto& task : all_tasks) {
        EXPECT_NE(output.find(std::to_string(task->get_id())), std::string::npos);
        EXPECT_NE(output.find(task->get_description()), std::string::npos);
    }
}

TEST_F(PrintTest, PrintSingleTask) {
    TaskManager manager(test_file);
    Task* task = manager.get_task(2);
    testing::internal::CaptureStdout();
    print_task(task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(std::to_string(2)), std::string::npos);
    EXPECT_NE(output.find("Test task 2"), std::string::npos);
    EXPECT_NE(output.find("IN_PROGRESS"), std::string::npos);
}

TEST_F(PrintTest, PrintAddedTask) {
    TaskManager manager(test_file);
    Task* new_task = manager.add_task("Newly added task");
    testing::internal::CaptureStdout();
    print_added_task(new_task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(std::to_string(6)), std::string::npos);
    EXPECT_NE(output.find("Newly added task"), std::string::npos);
}

TEST_F(PrintTest, PrintRemovedTask) {
    TaskManager manager(test_file);
    bool success = manager.remove_task(3);
    testing::internal::CaptureStdout();
    print_removed_task(3, success);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("removed successfully"), std::string::npos);
    // �����Ƴ������ڵ�����
    success = manager.remove_task(999);
    testing::internal::CaptureStdout();
    print_removed_task(999, success);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("not found"), std::string::npos);
}

TEST_F(PrintTest, PrintUpdatedTask) {
    TaskManager manager(test_file);
    Task* task = manager.update_task_description(4, "Updated description for task 4");
    testing::internal::CaptureStdout();
    print_updated_task(task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(std::to_string(4)), std::string::npos);
    EXPECT_NE(output.find("Updated description for task 4"), std::string::npos);
}

TEST_F(PrintTest, PrintGetTask) {
    TaskManager manager(test_file);
    Task* task = manager.get_task(1);
    testing::internal::CaptureStdout();
    print_get_task(task);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find(std::to_string(1)), std::string::npos);
    EXPECT_NE(output.find("Test task 1"), std::string::npos);
    // ���Ի�ȡ�����ڵ�����
    task = manager.get_task(9999);
    testing::internal::CaptureStdout();
    print_get_task(task);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Task not found"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}