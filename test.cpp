// test.cpp
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>  
#include <sstream>

#define private public
#include "TaskManager.hpp"
#undef private

static void WriteTaskFile(const std::string& content) {
    std::ofstream out("task.txt", std::ios::trunc);
    out << content;
}


TEST(TaskManagerFileTest, LoadFromEmptyFile_NoTasks) {
    WriteTaskFile("");

    TaskManager manager;

    EXPECT_EQ(manager.tasks.size(), 0u);
    EXPECT_EQ(manager.next_id, 1); 
}

TEST(TaskManagerFileTest, LoadFromFile_WithTasks) {
    WriteTaskFile(
        "1,0,Do homework\n"
        "2,1,Wash dishes\n"
    );

    TaskManager manager;

    ASSERT_EQ(manager.tasks.size(), 2u);

    EXPECT_TRUE(manager.tasks.count(1));
    EXPECT_TRUE(manager.tasks.count(2));

    EXPECT_EQ(manager.tasks[1].name, "Do homework");
    EXPECT_FALSE(manager.tasks[1].status);

    EXPECT_EQ(manager.tasks[2].name, "Wash dishes");
    EXPECT_TRUE(manager.tasks[2].status);

    EXPECT_EQ(manager.next_id, 3);
}

TEST(TaskManagerLogicTest, AddTask_IncreasesSizeAndAssignsCorrectId) {
    WriteTaskFile("");

    TaskManager manager;
    ASSERT_EQ(manager.tasks.size(), 0u);
    ASSERT_EQ(manager.next_id, 1);
    std::istringstream fakeInput("New task\n");
    auto* oldBuf = std::cin.rdbuf(fakeInput.rdbuf());

    manager.add_task();

    std::cin.rdbuf(oldBuf);

    ASSERT_EQ(manager.tasks.size(), 1u);
    ASSERT_TRUE(manager.tasks.count(1));
    EXPECT_EQ(manager.tasks.at(1).name, "New task");
    EXPECT_FALSE(manager.tasks.at(1).status);
    EXPECT_EQ(manager.next_id, 2);
}

TEST(TaskManagerLogicTest, MarkTaskAsDone_ChangesStatus) {
    WriteTaskFile("1,0,Task A\n");

    TaskManager manager;
    ASSERT_TRUE(manager.tasks.count(1));
    EXPECT_FALSE(manager.tasks[1].status);

    manager.mark_task_as_done(1);

    EXPECT_TRUE(manager.tasks[1].status);
}

TEST(TaskManagerLogicTest, MarkTaskAsDone_InvalidId_DoesNothing) {
    WriteTaskFile("1,0,Task A\n");

    TaskManager manager;
    ASSERT_EQ(manager.tasks.size(), 1u);

    manager.mark_task_as_done(42);    

    EXPECT_EQ(manager.tasks.size(), 1u);
    EXPECT_FALSE(manager.tasks[1].status);
}

TEST(TaskManagerLogicTest, DeleteTask_RemovesTask) {
    WriteTaskFile(
        "1,0,Task A\n"
        "2,1,Task B\n"
    );

    TaskManager manager;
    ASSERT_EQ(manager.tasks.size(), 2u);

    manager.delete_task_on_num(1);

    EXPECT_EQ(manager.tasks.size(), 1u);
    EXPECT_FALSE(manager.tasks.count(1));
    EXPECT_TRUE(manager.tasks.count(2));
}

TEST(TaskManagerFileTest, SaveToFile_WritesCorrectFormat) {
    {
        WriteTaskFile("");
        TaskManager manager;
        manager.tasks.clear();
        manager.tasks[1] = Task(false, "Task A");
        manager.tasks[2] = Task(true,  "Task B");
        manager.next_id = 3;

        manager.save_to_file();
    }

    std::ifstream in("task.txt");
    ASSERT_TRUE(in.is_open());

    std::string line1, line2;
    std::getline(in, line1);
    std::getline(in, line2);

    std::set<std::string> got{line1, line2};
    std::set<std::string> expected{
        "1,0,Task A",
        "2,1,Task B"
    };

    EXPECT_EQ(got, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
