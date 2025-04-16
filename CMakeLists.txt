cmake_minimum_required(VERSION 3.31...4.1)
project(todo_list
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------- Add CPM -----------

set(CPM_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake")
if(NOT EXISTS "${CPM_PATH}")
    file(DOWNLOAD
        "https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake"
        "${CPM_PATH}"
        STATUS status
        SHOW_PROGRESS
    )
    list(GET status 0 status_code)
    if(NOT status_code EQUAL 0)
        message(FATAL_ERROR "Could not download CPM.cmake")
    endif()
endif()

include(cmake/CPM.cmake)

# ------- Add TPLs ----------

# Download SDL3
CPMAddPackage(
    NAME SDL3
    GITHUB_REPOSITORY "libsdl-org/SDL"
    GIT_TAG "main"
)

# Download ImGui
CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY "ocornut/imgui"
    GIT_TAG "docking"
)

# Set Up ImGui OpenGL3 and SDL3 backends
add_library(imgui STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
)

# Find OpenGL
find_package(OpenGL REQUIRED)

target_include_directories(imgui PUBLIC
    ${SDL3_INCLUDE_DIRS}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC SDL3::SDL3 OpenGL::GL)
# ------- Our Code ----------

add_subdirectory(include)
add_subdirectory(src bin)

# -------- Copy Resources ---
file(COPY "res" DESTINATION "${CMAKE_BINARY_DIR}")

# -------- Testing ----------
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()