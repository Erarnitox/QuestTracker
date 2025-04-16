#pragma once

#include "TaskList.hpp"

#include <vector>

//-----------------------------------------
//
//-----------------------------------------
struct AppState {
    TaskList quests;
    char newQuestTitle[255]{};
    char newQuestDesc[1024]{};
    char newObjectiveText[255]{};
    std::vector<std::string> newQuestObjectives;
    int selectedQuest{ -1 };
    bool ShowCreationWindow{ false };
    int nextQuestId{ 1 };
};