#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>

class Task {
public:
    Task() = default;
    Task(bool status, const std::string &name); 
    bool status = false;
    std::string name;
};

class TaskManager {
public:
    TaskManager();
    ~TaskManager();

    void add_task();
    void mark_task_as_done(int id);
    void show_tasks() const;
    void count_of_tasks() const;
    void show_tasks_as_done() const;
    void delete_task_on_num(int id);

private:
    std::unordered_map<int, Task> tasks;
    int next_id = 1;

    void load_from_file();
    void save_to_file() const;
};
