add_executable(todo_list_app)
target_sources(todo_list_app PRIVATE
    main.cpp
    TaskList.cpp
)

target_compile_options(todo_list_app PRIVATE
    -Werror         # Treat all warnings as errors
    -Wall           # Enable all the most common warnings
    -Wextra         
    -Wconversion
    -Wshadow
)

# third party libs
target_link_libraries(todo_list_app PRIVATE imgui)
target_link_libraries(todo_list_app PRIVATE SQLiteCpp)
