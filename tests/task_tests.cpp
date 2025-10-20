#include <gtest/gtest.h>
#include "task-tracker/task.h"
#include "task-tracker/task_manager.h"
#include <thread>  // ���� std::this_thread::sleep_for
#include <chrono>  // ���� std::chrono::seconds

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

TEST(TaskTest, RemoveTask) {
    Task* task = new Task("T-005", "Task to be removed");
    // ȷ�������Ѵ���
    EXPECT_EQ(task->get_id(), "T-005");
    // �Ƴ�����
    task->remove();
    // ���� remove ����������������task ָ���������գ������ٷ�����
    // ��������ֻ��ȷ��û�б�������
    SUCCEED();
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
    manager.add_task("T-001", "Task 1");
    manager.add_task("T-002", "Task 2");
    // �����׼���
    testing::internal::CaptureStdout();
    manager.list_tasks();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("T-001"), std::string::npos); // Ӧ���� T-001
    EXPECT_NE(output.find("T-002"), std::string::npos); // Ӧ���� T-002
}

TEST(TaskManagerTest, ListTasksByStatus) {
    TaskManager manager;
    manager.add_task("T-009", "Task 1");
    manager.add_task("T-010", "Task 2");
    manager.update_task_status("T-010", "DONE");
    // �����׼���
    testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::TO_DO);
    std::string output_todo = testing::internal::GetCapturedStdout();
    EXPECT_NE(output_todo.find("T-009"), std::string::npos); // Ӧ���� T-009
    EXPECT_EQ(output_todo.find("T-010"), std::string::npos); // ��Ӧ���� T-010
    testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::DONE);
    std::string output_done = testing::internal::GetCapturedStdout();
    EXPECT_NE(output_done.find("T-010"), std::string::npos); // Ӧ���� T-010
    EXPECT_EQ(output_done.find("T-009"), std::string::npos); // ��Ӧ���� T-009
	testing::internal::CaptureStdout();
    manager.list_tasks(TaskStatus::IN_PROGRESS);
    std::string output_in_progress = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_in_progress.find("T-009"), std::string::npos); // ��Ӧ���� T-009
	EXPECT_EQ(output_in_progress.find("T-010"), std::string::npos); // ��Ӧ���� T-010
}

