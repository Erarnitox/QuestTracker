#include "TaskList.hpp"

#include <string>

//----------------------------------------
//
//----------------------------------------
void TaskList::autoSaveWorker() {
  while (m_running) {
    std::unique_lock<std::mutex> lock(m_save_mutex);
   
    m_save_cv.wait_for(lock, m_save_interval, [this] {
        return !m_running || m_dirty.load();
    });
    
    if(!m_running) {
      break;
    }

    if(m_dirty.load()) {
      try{
        saveDatabase();
      } catch (const std::exception& e) {
        // keep running
      }
    }
  }
}

//----------------------------------------
//
//----------------------------------------
TaskList::TaskList(const std::string& db_file, std::chrono::seconds save_interval) 
  : m_db(db_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), m_save_interval(save_interval) {
    m_tasks = std::vector<Task>();

    // initialize database
    m_db.exec("CREATE TABLE IF NOT EXISTS quests (id INTEGER PRIMARY KEY, title TEXT, state INTEGER, description TEXT, category TEXT);");
    m_db.exec("CREATE TABLE IF NOT EXISTS objectives (id INTEGER PRIMARY KEY, quest_id INTEGER, title TEXT);");
    
    // loading from database
    SQLite::Statement  query(m_db, "SELECT id, title, state, description, category FROM quests;");

    while(query.executeStep()) {
      Task task;
      task.m_id = query.getColumn(0).getInt();
      task.m_title = query.getColumn(1).getText();
      task.m_state = static_cast<TaskState>(query.getColumn(2).getInt());
      task.m_description = query.getColumn(3).getText();
      task.m_category = query.getColumn(4).getText();

      task.m_objectives = std::vector<std::string>();
      
      SQLite::Statement objectives_query(m_db, "SELECT title FROM objectives WHERE quest_id = ?;");
      objectives_query.bind(1, task.m_id);

      while(objectives_query.executeStep()) {
        task.m_objectives.push_back(objectives_query.getColumn(0).getText());
      }

      m_tasks.push_back(task);
    }

    m_save_thread = std::make_unique<std::thread>(&TaskList::autoSaveWorker, this);
}

//----------------------------------------
//
//----------------------------------------
void TaskList::saveDatabase() {
  try{
    SQLite::Transaction trans(m_db);

    // clean database
    m_db.exec("DROP TABLE objectives;");
    m_db.exec("DROP TABLE quests;");

    // set up database
    m_db.exec("CREATE TABLE quests (id INTEGER PRIMARY KEY, title TEXT, state INTEGER, description TEXT, category TEXT);");
    m_db.exec("CREATE TABLE objectives (id INTEGER PRIMARY KEY, quest_id INTEGER, title TEXT);");

    // save to database
    for(const auto& quest : m_tasks) {
      SQLite::Statement insert(m_db, "INSERT INTO quests (id, title, state, description, category) VALUES (?, ?, ?, ?, ?);");
      insert.bind(1, quest.m_id);
      insert.bind(2, quest.m_title);
      insert.bind(3, static_cast<int>(quest.m_state));
      insert.bind(4, quest.m_description);
      insert.bind(5, quest.m_category);
      insert.exec();

      for(const auto& obj : quest.m_objectives) {
        SQLite::Statement insert_obj(m_db, "INSERT INTO objectives (quest_id, title) VALUES (?, ?);");
        insert_obj.bind(1, quest.m_id);
        insert_obj.bind(2, obj);
        insert_obj.exec();
      }
    }

    trans.commit();
    m_dirty = false;
    m_last_save_successful = true;
  } catch(std::exception& e) {
    m_last_save_successful = false;
  }
}

//----------------------------------------
//
//----------------------------------------
TaskList::~TaskList() {
  stopAutoSave();
  saveNow();
}

//----------------------------------------
//
//----------------------------------------
void TaskList::addTask(const Task& task) {
  m_tasks.push_back(task);
  markDirty();
}

//----------------------------------------
//
//----------------------------------------
void TaskList::markDirty() {
  m_dirty = true;
  m_save_cv.notify_one();
}

//----------------------------------------
//
//----------------------------------------
void TaskList::stopAutoSave() {
  m_running = false;
  m_save_cv.notify_one();

  if(m_save_thread && m_save_thread->joinable()) {
    m_save_thread->join();
  }
}

//----------------------------------------
//
//----------------------------------------
void TaskList::saveNow() {
  std::lock_guard<std::mutex> lock(m_save_mutex);

  if(m_dirty.load()) {
    saveDatabase();
  }
}
    
//----------------------------------------
//
//----------------------------------------
std::vector<Task>& TaskList::getTasks() noexcept {
  markDirty();
  return m_tasks;
}
