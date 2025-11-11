#pragma once

#include <condition_variable>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

#include <SQLiteCpp/SQLiteCpp.h>

class TaskList;

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
    explicit TaskList(const std::string& db_file = "quests.db", std::chrono::seconds save_interval = std::chrono::seconds(30));
    TaskList(const TaskList&) = delete;
    TaskList& operator=(const TaskList&) = delete;
    ~TaskList();

    void addTask(const Task& task); 
    std::vector<Task>& getTasks() noexcept;
    void saveNow();

  private:
    SQLite::Database m_db;
    std::chrono::seconds m_save_interval;
    std::unique_ptr<std::thread> m_save_thread;
    std::atomic<bool> m_running;
    std::atomic<bool> m_dirty;
    std::atomic<bool> m_last_save_successful;
    std::mutex m_save_mutex;
    std::condition_variable m_save_cv;
    std::vector<Task> m_tasks;

    void autoSaveWorker();
    void saveDatabase();
    void stopAutoSave();
    void markDirty();
};
