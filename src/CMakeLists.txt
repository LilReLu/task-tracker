set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(task_cli_lib STATIC
    task.cpp
    task_manager.cpp)

target_include_directories(task_cli_lib PUBLIC
    "${CMAKE_SOURCE_DIR}/include/task-tracker"
)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(task_cli_lib PUBLIC
    nlohmann_json::nlohmann_json
)

add_executable(${PROJECT_NAME} main.cpp "task_manager.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE task_cli_lib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

