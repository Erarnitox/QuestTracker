#include <memory>

#include "../src/TaskList.hpp"

#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <assert.h>
#include <filesystem>
#include <memory>

//----------------------------------------
//
//----------------------------------------
struct TaskListFixture {
  const std::string test_db_file = "test_quests.db";
  std::unique_ptr<TaskList> taskList;

  TaskListFixture() {
    // clean up the old database file
    if(std::filesystem::exists(test_db_file)) {
      std::filesystem::remove(test_db_file);
    }
    taskList = make_unique<TaskList>(test_db_file);
  }
};

//----------------------------------------
//
//----------------------------------------
TEST_CASE_METHOD(TaskListFixture, "Basic Operations of the TaskList", "[TaskList]") {
  SECTION("Add new Task") {
    constexpr auto title{ "Buy groceries" };
    constexpr auto state{ TASK_NOT_STARTED };
    constexpr auto description{ "Milk, Bread, Eggs" };
    constexpr auto category{ "Misc" };

    Task task;
    task.m_title = title;
    task.m_state = state;
    task.m_description = description;
    task.m_objectives = { "Go to supermarket", "Find stuff", "pay", "Go Home" };
    task.m_category = category; 

    taskList->addTask(task);

    REQUIRE(taskList->getTasks().size() == 1);
    
    const auto& retTask = taskList->getTasks().at(0);
    REQUIRE(retTask.m_title == title); 
    REQUIRE(retTask.m_state == state); 
    REQUIRE(retTask.m_description == description); 
    REQUIRE(retTask.m_category == category); 

    const auto& objectives = retTask.m_objectives;
    REQUIRE(std::find(objectives.begin(), objectives.end(), "pay") != objectives.end());
  }

  SECTION("Empty TaskList") {
    REQUIRE(taskList->getTasks().empty());
  }
}
