cmake_minimum_required(VERSION 3.15)
project(task_gtest_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

add_executable(task_tests
	task_tests.cpp
)

target_link_libraries(task_tests PRIVATE
	GTest::gtest_main
	task_cli_lib
)

# 包含 GTest 的头文件
target_include_directories(task_tests PRIVATE
    ${GTEST_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(task_tests)