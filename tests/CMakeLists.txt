enable_testing()
add_executable(tests tests.cpp)
add_test(NAME UnitTests COMMAND tests)

target_link_libraries(tests PRIVATE task_list)