cmake_minimum_required(VERSION 3.16)
project(TaskManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(taskmanager
    src/TaskManager.cpp
    lib/TaskManager.hpp
)

target_include_directories(taskmanager
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

add_executable(taskmanager_app
    main.cpp
)

target_link_libraries(taskmanager_app
    taskmanager
)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK  OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(taskmanager_tests
    test.cpp
)

target_link_libraries(taskmanager_tests
    taskmanager
    gtest
    gtest_main
)

enable_testing()
add_test(NAME TaskManagerTests COMMAND taskmanager_tests)
