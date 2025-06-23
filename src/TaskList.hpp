#pragma once

#include <string>
#include <vector>

#include <SQLiteCpp/SQLiteCpp.h>

//-----------------------------------------
//
//-----------------------------------------
enum TaskState {
    TASK_NOT_STARTED = 0,
    TASK_IN_PROGRESS,
    TASK_COMPLETED
};

//-----------------------------------------
//
//-----------------------------------------
struct Task {
    int m_id;
    std::string m_title;
    TaskState m_state;
    std::string m_description;
    std::vector<std::string> m_objectives;
    std::string m_category;
};

//-----------------------------------------
//
//-----------------------------------------
class TaskList {
  public:
    explicit TaskList(const std::string& db_file = "quests.db");
    ~TaskList();
    void addTask(const Task& task); 
    std::vector<Task>& getTasks() noexcept;
  private:
    SQLite::Database m_db;
    std::vector<Task> m_tasks;
};
