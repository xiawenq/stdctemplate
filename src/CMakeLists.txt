project(main)

add_executable(${PROJECT_NAME} main.c)

target_include_directories(${PROJECT_NAME} PRIVATE ../3rd/argtable3/src)

target_link_libraries(${PROJECT_NAME} easylogger pthread argtable3)