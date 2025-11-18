#include "TaskManager.hpp"

Task::Task(bool status, const std::string& name)
    : status(status), name(name) {}

TaskManager::TaskManager() {
    load_from_file();
}

TaskManager::~TaskManager() {
    save_to_file();
}

void TaskManager::load_from_file() {
    std::ifstream in("task.txt");
    if (!in.is_open()) {
        return;
    }

    std::string line;
    int max_id = 0;

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::string id_str, status_str, name;

        auto pos1 = line.find(',');
        auto pos2 = line.find(',', pos1 + 1);
        id_str = line.substr(0, pos1);
        status_str = line[pos2 - 1];
        name = line.substr(pos2 + 1);

        int id = std::stoi(id_str);
        bool status = (status_str == "1");

        tasks[id] = Task(status, name);
        if (id > max_id) max_id = id;
    }

    next_id = max_id + 1;
}

void TaskManager::save_to_file() const {
    std::ofstream out("task.txt");
    if (!out.is_open()) {
        std::cerr << "Не удалось открыть task.txt для записи\n";
        return;
    }

    for (const auto& [id, task] : tasks) {
        out << id << ',' << (task.status ? 1 : 0) << ',' << task.name << '\n';
    }
}


void TaskManager::add_task() {
    std::cout << "Введите название задачи: ";
    std::string name;
    std::getline(std::cin, name);

    tasks[next_id] = Task(false, name);
    std::cout << "Задача добавлена под номером " << next_id << "\n";
    ++next_id;
}

void TaskManager::mark_task_as_done(int id) {
    if (!(id)) {
        std::cout << "Некорректный id.\n";
        return;
    }

    auto it = tasks.find(id);
    if (it == tasks.end()) {
        std::cout << "Задача с таким номером не найдена.\n";
        return;
    }

    it->second.status = true;
    std::cout << "Задача " << id << " отмечена как выполненная.\n";
}

void TaskManager::show_tasks() const {
    if (tasks.empty()) {
        std::cout << "Список задач пуст.\n";
        return;
    }

    std::cout << "Все задачи:\n";
    for (const auto& [id, task] : tasks) {
        std::cout << id << ", " << (task.status ? "+ " : "- ") << ", " << task.name << '\n';
    }
}

void TaskManager::count_of_tasks() const {
    std::cout << "Всего задач: " << tasks.size() << '\n';
}

void TaskManager::show_tasks_as_done() const {
    bool any = false;
    for (const auto &[id, task]: tasks) {
        if (task.status) {
            if (!any) {
                std::cout << "Выполненные задачи:\n";
                any = true;
            }
            std::cout << id << ". + " << task.name << '\n';
        }
    }
    if (!any) {
        std::cout << "Нет выполненных задач.\n";
    }
}

void TaskManager::delete_task_on_num(int id) {
    if (!(id)) {
        std::cout << "Некорректный id.\n";
        return;
    }

    auto it = tasks.find(id);
    if (it == tasks.end()) {
        std::cout << "Задача с таким номером не найдена.\n";
        return;
    }

    tasks.erase(it);
    std::cout << "Задача " << id << " удалена.\n";
}

int main () {
    TaskManager a = TaskManager();
    a.show_tasks();
    a.delete_task_on_num(1);
    a.add_task();
    a.add_task();
    a.show_tasks();
}