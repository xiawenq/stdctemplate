cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_STANDARD 99)

project(easylogger C)

add_library(${PROJECT_NAME}
        ../../3rd/EasyLogger/easylogger/src/elog.c
        ../../3rd/EasyLogger/easylogger/src/elog_async.c
        ../../3rd/EasyLogger/easylogger/src/elog_buf.c
        ../../3rd/EasyLogger/easylogger/src/elog_utils.c
        ../../3rd/EasyLogger/easylogger/plugins/file/elog_file.c

        port/elog_file_port.c
        port/elog_port.c
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        inc
        ../../3rd/EasyLogger/easylogger/inc
        ../../3rd/EasyLogger/easylogger/plugins/file
        )