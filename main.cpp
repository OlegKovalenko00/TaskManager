#include "TaskManager.hpp"

int main () {
    TaskManager a = TaskManager();
    a.show_tasks();
    a.delete_task_on_num(1);
    a.add_task();
    a.add_task();
    a.show_tasks();
}
// проверка ямл