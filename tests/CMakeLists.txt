cmake_minimum_required(VERSION 3.15)
project(task_gtest_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(run_tests
	task.cpp
	manager.cpp
	ui.cpp)

target_link_libraries(run_tests PRIVATE
	GTest::gtest_main
	task_cli_lib
)

# 包含 GTest 的头文件
target_include_directories(run_tests PRIVATE
    ${GTEST_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(run_tests)