#include "TaskList.hpp"

#include <string>

//----------------------------------------
//
//----------------------------------------
TaskList::TaskList(const std::string& db_file) : m_db(db_file, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
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
}

//----------------------------------------
//
//----------------------------------------
TaskList::~TaskList() {
  // clean database
  SQLite::Statement drop_objectives(m_db, "DROP TABLE objectives;");
  drop_objectives.exec();

  SQLite::Statement drop_quests(m_db, "DROP TABLE quests;");
  drop_quests.exec();

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
}

//----------------------------------------
//
//----------------------------------------
void TaskList::addTask(const Task& task) {
  m_tasks.push_back(task);
}
    
//----------------------------------------
//
//----------------------------------------
std::vector<Task>& TaskList::getTasks() noexcept {
  return m_tasks;
}
