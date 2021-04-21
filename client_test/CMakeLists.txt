# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

cmake_minimum_required(VERSION 3.17)
project(chatty_client)

set(CMAKE_CXX_STANDARD)


set(PROJECT_FILE_HEADER
        "../chatty_test_client.hpp"
        )

set(PROJECT_FILE_CONTENT
        "../chatty_test_client.cpp"
        )

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

add_executable(chatty_client ${PROJECT_FILE_HEADER} ${PROJECT_FILE_CONTENT})

