include(CTest)

add_executable(tests 
  tests.cpp
  ../src/TaskList.cpp
)

CPMAddPackage("gh:catchorg/Catch2@3.4.0")

target_link_libraries(tests Catch2::Catch2WithMain)
target_link_libraries(tests SQLiteCpp)

enable_testing()
add_test(NAME UnitTests COMMAND tests)
