cmake_minimum_required(VERSION 3.17)
project(chatty)

set(CMAKE_CXX_STANDARD 14)

#find_library(
#        sqlite3_library
#        NAMES sqlite3.so
#        HINTS /usr/lib /usr/local/lib
#        REQUIRED
#)
#message(STATUS sqlite3_path : ${sqlite3_library})

if (WIN32)
    #C:\Program Files\DB Browser for SQLite\sqlite3.dll
endif()

#CONST VARIABLE DEFINES - OPEN

add_definitions(-DHELLO_CHATTY=1)
set(PROJECT_DEFS_PREFIX CHATTY_)                            # [DO-NOT-CHANGE-THIS-SHIT-VARIABLE. PLEASE!]

set(${PROJECT_DEFS_PREFIX}DB_NAME "chatty_database_storage")        # CHANGE THIS FOR YOUR CUSTOMS
set(${PROJECT_DEFS_PREFIX}DB_EXTENSION ".db")



set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 5)

set(PROJECT_FILE_HEADER
        "chatty_network_master.h"
        "chatty.h"
        "chatty_embedded_db_includes.h"
        "chatty_embedded_db_type_defs.h"
        "chatty_embedded_db_interface.h"
        "chatty_embedded_db_result.h"
        "chatty_embedded_db_request.h"
        "chatty_embedded_db_master.h"
        "chatty_embedded_db_controller.h"
        "chatty_defs.h"
        "chatty_server_main.hpp"
        )

set(PROJECT_FILE_CONTENT
        "chatty_network_master.cpp"
        "chatty_embedded_db_interface.cpp"
        "chatty_embedded_db_result.cpp"
        "chatty_embedded_db_request.cpp"
        "chatty_embedded_db_master.cpp"
        "chatty_embedded_db_controller.cpp"
        "chatty_server_main.cpp"
        )

#CONST VARIABLE DEFINES - END


#MAKE CONFIGURE HEADER FILE
message(STATUS Configure_chatty_defs)
configure_file(chatty_defs.h.in ${PROJECT_SOURCE_DIR}/chatty_defs.h)

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

#ADD EXECUABLE
add_executable(chatty ${PROJECT_FILE_HEADER} ${PROJECT_FILE_CONTENT})

#ADD_CUSTOM_COMMAND(TARGET chatty
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/${${PROJECT_DEFS_PREFIX}DB_NAME}${${PROJECT_DEFS_PREFIX}DB_EXTENSION} ${PROJECT_BINARY_DIR}
#        )


#ADD DEPENDENCE
target_link_libraries(chatty PRIVATE sqlite3)

target_compile_definitions(chatty PRIVATE CHATTY_DO_BUILD_DEBUG)