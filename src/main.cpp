#include "AppState.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_opengl.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"

#include "TaskList.hpp"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_opengl3.h"
#include <algorithm>
#include <cstring>

//-----------------------------------------
//
//-----------------------------------------
void set_erarnitox_style() {
    ImVec4* colors = ImGui::GetStyle().Colors;

    // Convert brand colors to ImVec4
    const ImVec4 main_bg(0.11f, 0.114f, 0.133f, 1.00f);        // #1C1D22
    const ImVec4 sec_bg(0.192f, 0.231f, 0.267f, 1.00f);        // #313B44
    const ImVec4 highlight(1.00f, 0.184f, 0.463f, 1.00f);      // #ff2f76
    const ImVec4 form_bg(0.745f, 0.314f, 0.459f, 1.00f);       // #be5075
    const ImVec4 main_text(0.667f, 0.671f, 0.678f, 1.00f);     // #AAABAD
    const ImVec4 sec_text(0.365f, 0.486f, 0.514f, 1.00f);      // #5D7C83
    const ImVec4 white(0.988f, 0.992f, 1.00f, 1.00f);          // #fcfdff

    // Core colors
    colors[ImGuiCol_Text]                   = main_text;
    colors[ImGuiCol_TextDisabled]           = sec_text;
    colors[ImGuiCol_WindowBg]               = main_bg;
    colors[ImGuiCol_ChildBg]                = main_bg;
    colors[ImGuiCol_PopupBg]                = sec_bg;
    colors[ImGuiCol_Border]                 = sec_text;
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = sec_bg;
    colors[ImGuiCol_FrameBgHovered]         = highlight;
    colors[ImGuiCol_FrameBgActive]          = form_bg;

    // Interactive elements
    colors[ImGuiCol_TitleBg]                = sec_bg;
    colors[ImGuiCol_TitleBgActive]          = form_bg;
    colors[ImGuiCol_TitleBgCollapsed]       = sec_bg;
    colors[ImGuiCol_MenuBarBg]              = sec_bg;
    colors[ImGuiCol_ScrollbarBg]            = sec_bg;
    colors[ImGuiCol_ScrollbarGrab]          = form_bg;
    colors[ImGuiCol_ScrollbarGrabHovered]   = highlight;
    colors[ImGuiCol_ScrollbarGrabActive]    = highlight;
    colors[ImGuiCol_CheckMark]              = white;
    colors[ImGuiCol_SliderGrab]             = form_bg;
    colors[ImGuiCol_SliderGrabActive]       = highlight;
    colors[ImGuiCol_Button]                 = form_bg;
    colors[ImGuiCol_ButtonHovered]          = highlight;
    colors[ImGuiCol_ButtonActive]           = form_bg;
    colors[ImGuiCol_Header]                 = form_bg;
    colors[ImGuiCol_HeaderHovered]          = highlight;
    colors[ImGuiCol_HeaderActive]           = form_bg;

    // Secondary UI elements
    colors[ImGuiCol_Separator]              = sec_text;
    colors[ImGuiCol_SeparatorHovered]       = highlight;
    colors[ImGuiCol_SeparatorActive]        = form_bg;
    colors[ImGuiCol_ResizeGrip]             = sec_text;
    colors[ImGuiCol_ResizeGripHovered]      = highlight;
    colors[ImGuiCol_ResizeGripActive]       = form_bg;
    colors[ImGuiCol_Tab]                    = sec_bg;
    colors[ImGuiCol_TabHovered]             = highlight;
    colors[ImGuiCol_TabActive]              = form_bg;
    colors[ImGuiCol_TabUnfocused]           = sec_bg;
    colors[ImGuiCol_TabUnfocusedActive]     = form_bg;

    // Special purpose
    colors[ImGuiCol_TextSelectedBg]         = highlight;
    colors[ImGuiCol_DragDropTarget]         = highlight;
    colors[ImGuiCol_NavHighlight]           = highlight;

    // Style adjustments
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding     = ImVec2(12.00f, 12.00f);
    style.FramePadding      = ImVec2(8.00f, 4.00f);
    style.CellPadding       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing       = ImVec2(8.00f, 6.00f);
    style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
    style.IndentSpacing     = 25;
    style.ScrollbarSize     = 16;
    style.GrabMinSize       = 12;

    // Border sizes
    style.WindowBorderSize  = 0.8f;
    style.ChildBorderSize   = 0.8f;
    style.PopupBorderSize   = 0.8f;
    style.FrameBorderSize   = 0.8f;
    style.TabBorderSize     = 0.8f;

    // Rounding
    style.WindowRounding    = 4.0f;
    style.ChildRounding     = 4.0f;
    style.FrameRounding     = 2.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 2.0f;
    style.TabRounding       = 4.0f;
}

//-----------------------------------------
//
//-----------------------------------------
void ShowCreationWindow(AppState& appState) {
    if(not appState.ShowCreationWindow) return;

    ImGui::OpenPopup("Create New Quest");
    if(ImGui::BeginPopupModal("Create New Quest", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        // Title input:
        ImGui::InputText("Title", 
            appState.newQuestTitle, 
            IM_ARRAYSIZE(appState.newQuestTitle)
        );

        // Description input:
        ImGui::InputTextMultiline("Description", 
            appState.newQuestDesc, 
            IM_ARRAYSIZE(appState.newQuestDesc)
        );

        // Objectives:
        ImGui::SeparatorText("Objectives");
        for(size_t i{ 0 }; i < appState.newQuestObjectives.size(); ++i) {
            ImGui::PushID(static_cast<int>(i));
            ImGui::SameLine();
            if(ImGui::Button("X")) {
                appState.newQuestObjectives.erase(
                    appState.newQuestObjectives.begin() + static_cast<int>(i)
                );
            }
            ImGui::PopID();
        }

        // Add new objectives
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##newobj", 
            appState.newObjectiveText, 
            IM_ARRAYSIZE(appState.newObjectiveText)
        );
        ImGui::SameLine();
        if(ImGui::Button("Add Objective")) {
            if(strlen(appState.newObjectiveText) > 0) {
                appState.newQuestObjectives.emplace_back(appState.newObjectiveText);
                memset(appState.newObjectiveText, 0, sizeof(appState.newObjectiveText));
            }
        }

        // Category Input:
        static const char* categories[] = {
            "Main Quest",
            "Side Quest",
            "Faction Quest",
            "Misc"
        };

        static int currentCategory{ 0 };
        ImGui::Combo("Category", 
            &currentCategory, 
            categories, 
            IM_ARRAYSIZE(categories)
        );

        // Create Button
        if(ImGui::Button("Create")) {
            Task newQuest{
                .m_id=appState.nextQuestId++,
                .m_title=appState.newQuestTitle,
                .m_state=TASK_NOT_STARTED,
                .m_description=appState.newQuestDesc,
                .m_objectives=appState.newQuestObjectives,
                .m_category=categories[currentCategory]
            };

            appState.quests.addTask(newQuest);

            // Reset input fields
            memset(appState.newQuestTitle, 0, sizeof(appState.newQuestTitle));
            memset(appState.newQuestDesc, 0, sizeof(appState.newQuestDesc));
            appState.newQuestObjectives.clear();
            appState.ShowCreationWindow = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
            appState.ShowCreationWindow = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

}

//-----------------------------------------
//
//-----------------------------------------
void ShowQuestTracker(AppState& appState) {
    // Add Quest Button
    if(ImGui::Button("+ New Quest")) {
        appState.ShowCreationWindow = true;
    }

    ImGui::Separator();
    // Quest Overview
    for(auto& quest : appState.quests) {
        ImGui::PushID(quest.m_id);

        // Quest header
        ImGui::PushStyleColor(ImGuiCol_Header, 
            quest.m_state == TASK_COMPLETED ? ImVec4(0.2f, 0.6f, 0.2f, 0.7f) :
            quest.m_state == TASK_IN_PROGRESS ? ImVec4(0.2f, 0.4f, 0.8f, 0.7f) :
            ImVec4(0.5f, 0.5f, 0.5f, 0.7f));

        if(ImGui::CollapsingHeader(quest.m_title.c_str())) {
            ImGui::Indent();
            ImGui::TextWrapped("Description: %s", quest.m_description.c_str());
            ImGui::Text("Category: %s", quest.m_category.c_str());

            // Objectives
            ImGui::SeparatorText("Objectives");
            for(const auto& objective : quest.m_objectives) {
                ImGui::BulletText("%s", objective.c_str());
            }

            ImGui::Separator();
            // State:
            if(ImGui::Button(quest.m_state == TASK_COMPLETED ? "Completed!" : "Mark Complete")) {
                quest.m_state = TASK_COMPLETED;
            }

            // Delete Button:
            ImGui::SameLine();
            if(ImGui::Button("Delete")) {
                appState.quests.erase(
                    std::ranges::remove_if(appState.quests, [&](const Task& task){
                        return task.m_id == quest.m_id;
                    }).begin()
                );
                ImGui::PopStyleColor();
                ImGui::PopID();
                break;
            }

            ImGui::Unindent();
        }

        ImGui::PopStyleColor();
        ImGui::PopID();
    }
}

//-----------------------------------------
//
//-----------------------------------------
int main() {
    // Setup SDL
    if(not SDL_Init(SDL_INIT_VIDEO)) {
        std::println("SDL_Init Error: {}", SDL_GetError());
        return -1;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version{ "#version 130" };
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create a window
    SDL_Window* window{ SDL_CreateWindow("Erarnitox's Quest Tracker", 800, 1000, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)};
    SDL_GLContext gl_context{ SDL_GL_CreateContext(window) };
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ ImGui::GetIO() };

    // Setup Backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //TODO: styling
    ImFont* erarnitox_font{ io.Fonts->AddFontFromFileTTF("../res/LeroyLettering.ttf", 20) };
    set_erarnitox_style();

    AppState appState;

    bool done{ false };
    while(not done) {
        // Poll and handle events
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            
            if(event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }

        // Drawing
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(erarnitox_font);

        // Draw our window
        ImGui::SetNextWindowPos({0, 0});
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Erarnitox's Quest Tracker", nullptr, 
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoSavedSettings
        );

        // Main Content
        ImGui::BeginChild("Content", {}, true);
        {
            ShowCreationWindow(appState);
            ShowQuestTracker(appState);
        }
        ImGui::EndChild();
        ImGui::End();
        ImGui::PopFont();

        // Rendering
        ImGui::Render();
        glViewport(
            0, 
            0, 
            static_cast<GLint>(io.DisplaySize.x), 
            static_cast<GLint>(io.DisplaySize.y)
        );
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();
    SDL_GL_DestroyContext(gl_context);
    SDL_Quit();
}