#include <gtest/gtest.h>
#include "task-tracker/task.h"
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