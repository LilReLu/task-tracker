#include <gtest/gtest.h>
#include "task-tracker/task.h"
#include "task-tracker/task_manager.h"
#include <thread>  // ���� std::this_thread::sleep_for
#include <chrono>  // ���� std::chrono::seconds

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

// ���Թ��캯���Ƿ���ȷ�����˳�ʼ״̬
TEST(TaskTest, ConstructorInitialization) {
    std::string id = "T-001";
    std::string desc = "Implement the core feature";

    Task task(id, desc);

    // ����ʼֵ
    EXPECT_EQ(task.get_id(), id);
    EXPECT_EQ(task.get_description(), desc);
    EXPECT_EQ(task.get_status(), TaskStatus::TO_DO); // Ĭ��״̬ӦΪ TO_DO

    // ʱ���Ӧ�ñ�����
    EXPECT_NE(task.get_created_at(), 0);
    // �մ���ʱ��updated_at Ӧ���� created_at
    EXPECT_EQ(task.get_created_at(), task.get_updated_at());
}

TEST(TaskTest, UpdateStatus) {
    Task task("T-002", "Write unit tests");
    // ��ʼ״̬ӦΪ TO_DO
    EXPECT_EQ(task.get_status(), TaskStatus::TO_DO);
    // ����״̬Ϊ IN_PROGRESS
    task.update_status("IN_PROGRESS");
    EXPECT_EQ(task.get_status(), TaskStatus::IN_PROGRESS);
    // ����״̬Ϊ DONE
    task.update_status("DONE");
    EXPECT_EQ(task.get_status(), TaskStatus::DONE);
    // ����������Ч״̬��Ӧ����ԭ״̬����
    task.update_status("INVALID_STATUS");
    EXPECT_EQ(task.get_status(), TaskStatus::DONE);
}

TEST(TaskTest, UpdateDescription) {
    Task task("T-003", "Initial description");
	EXPECT_EQ(task.get_description(), "Initial description");
    std::string new_desc = "Updated description";
    // ��������
    task.update_description(new_desc);
    EXPECT_EQ(task.get_description(), new_desc);
}

TEST(TaskTest, TimeUpdateOnModification) {
    Task task("T-004", "Check timestamps");
    std::time_t created_at = task.get_created_at();
    std::time_t updated_at = task.get_updated_at();
    // �մ���ʱ��created_at �� updated_at Ӧ���
    EXPECT_EQ(created_at, updated_at);
    // �ȴ�һ������ȷ��ʱ�����ı�
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // ����������Ӧ�ø��� updated_at
    task.update_description("New description");
    std::time_t new_updated_at = task.get_updated_at();
    EXPECT_GT(new_updated_at, updated_at); // updated_at Ӧ�ø���
    // �ٴεȴ�һ����
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // ����״̬��Ӧ���ٴθ��� updated_at
    task.update_status("IN_PROGRESS");
    std::time_t latest_updated_at = task.get_updated_at();
    EXPECT_GT(latest_updated_at, new_updated_at); // updated_at Ӧ���ٴθ���
}

TEST(TaskManagerTest, AddAndGetTask) {
    TaskManager manager;
	Task* empty_task = manager.get_task("NON_EXISTENT");
	EXPECT_EQ(empty_task, nullptr); // ȷ�������ڵ����񷵻� nullptr
    manager.add_task("T-006", "Manage tasks effectively");
    Task* task = manager.get_task("T-006");
    ASSERT_NE(task, nullptr); // ȷ��������ȷ���
    EXPECT_EQ(task->get_id(), "T-006");
    EXPECT_EQ(task->get_description(), "Manage tasks effectively");
}

TEST(TaskManagerTest, RemoveTask) {
    TaskManager manager;
    manager.add_task("T-007", "Task to be removed from manager");
    Task* task = manager.get_task("T-007");
    ASSERT_NE(task, nullptr); // ȷ��������ȷ���
    manager.remove_task("T-007");
    Task* removed_task = manager.get_task("T-007");
    EXPECT_EQ(removed_task, nullptr); // ȷ��������ȷ�Ƴ�
}

TEST(TaskManagerTest, UpdateTaskStatusAndDescription) {
    TaskManager manager;
    manager.add_task("T-008", "Initial description");
    manager.update_task_status("T-008", "IN_PROGRESS");
    manager.update_task_description("T-008", "Updated description");
    Task* task = manager.get_task("T-008");
    ASSERT_NE(task, nullptr); // ȷ���������
    EXPECT_NE(task->get_status(), TaskStatus::TO_DO);
    EXPECT_EQ(task->get_status(), TaskStatus::IN_PROGRESS);
    EXPECT_NE(task->get_description(), "Initial description");
    EXPECT_EQ(task->get_description(), "Updated description");
}

TEST(TaskManagerTest, ListAllTasks) {
    TaskManager manager;
    manager.add_task("T-0011", "Task 11");
    manager.add_task("T-0012", "Task 12");
    // �����׼���
    testing::internal::CaptureStdout();
    manager.list_tasks();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-0011"), std::string::npos); // Ӧ���� T-0011
    EXPECT_NE(output.find("T-0012"), std::string::npos); // Ӧ���� T-0012
}

TEST_F(FilereaderTest, ListTasksByStatu) {
    TaskManager manager("test_tasks.json");
    // �����׼���
    testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::TO_DO);
    std::string output = testing::internal::GetCapturedStdout();
    // Ӧ���� T-001 �� T-004
    EXPECT_NE(output.find("T-001"), std::string::npos);
    EXPECT_NE(output.find("T-004"), std::string::npos);
    // ��Ӧ���� T-002 �� T-003
    EXPECT_EQ(output.find("T-002"), std::string::npos);
	EXPECT_EQ(output.find("T-003"), std::string::npos);

	testing::internal::CaptureStdout();
	manager.list_tasks(TaskStatus::IN_PROGRESS);
    output = testing::internal::GetCapturedStdout();
    // Ӧ���� T-002
    EXPECT_NE(output.find("T-002"), std::string::npos);
    // ��Ӧ���� T-001��T-003 �� T-004
    EXPECT_EQ(output.find("T-001"), std::string::npos);
    EXPECT_EQ(output.find("T-003"), std::string::npos);
	EXPECT_EQ(output.find("T-004"), std::string::npos);

	testing::internal::CaptureStdout();
	manager.list_tasks(TaskStatus::DONE);
    output = testing::internal::GetCapturedStdout();
    // Ӧ���� T-003
    EXPECT_NE(output.find("T-003"), std::string::npos);
    // ��Ӧ���� T-001��T-002 �� T-004
    EXPECT_EQ(output.find("T-001"), std::string::npos);
    EXPECT_EQ(output.find("T-002"), std::string::npos);
	EXPECT_EQ(output.find("T-004"), std::string::npos);
}

TEST_F(FilereaderTest, LoadFromFile) {
    TaskManager manager("test_tasks.json");
    // ��������Ƿ���ȷ����
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
	// ����һ�����ļ�
	std::ofstream ofs(empty_filename);
    ASSERT_TRUE(ofs.is_open());
    ofs.close(); // �����رգ�ʹ��Ϊ��

	TaskManager manager(empty_filename);
    // ȷ��û�����񱻼���
	EXPECT_EQ(manager.IsEmpty(), true);
    // ɾ�������ļ�
	std::remove(empty_filename.c_str());
}
TEST_F(FilereaderTest, FileCreationIfNotExist) {
    std::string non_existent_file = "non_existent_test_file.json";
    // ȷ���ļ�������
    std::remove(non_existent_file.c_str());
    // ���� TaskManager ʵ����Ӧ�ûᴴ���ļ�
    TaskManager manager(non_existent_file);
    // ����ļ��Ƿ񱻴���    
    std::ifstream ifs(non_existent_file);
    EXPECT_TRUE(ifs.is_open()) << "File was not created.";
    ifs.close();
    // ɾ�������ļ�
    std::remove(non_existent_file.c_str());
}

TEST_F(FilereaderTest, SaveToFile) {
    TaskManager manager("test_tasks.json");
    // ���һ��������
    manager.add_task("T-005", "Newly added task");
    // ���浽�ļ�
    manager.update_task_status("T-005", "IN_PROGRESS");
    manager.update_task_description("T-005", "Updated description for new task");
    // ���¼����ļ�����֤����
    TaskManager reloaded_manager("test_tasks.json");
    Task* task = reloaded_manager.get_task("T-005");
    ASSERT_NE(task, nullptr); // ȷ���������
    EXPECT_EQ(task->get_description(), "Updated description for new task");
    EXPECT_EQ(task->get_status(), TaskStatus::IN_PROGRESS);
}
