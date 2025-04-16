#pragma once

#include <print>
#include <string>
#include <memory>
#include <list>
#include <vector>

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
class TaskList : public std::vector<Task> {
public:
    void addTask(const Task& task) {
        this->push_back(task);
    }
};