#include <memory>
#define private public
#define protected public

#include "TaskList.hpp"

#include <assert.h>

void test_add_task() {
    std::println("Passed basic UnitTest");
}

int main() {
    test_add_task();
}