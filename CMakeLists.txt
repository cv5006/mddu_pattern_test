cmake_minimum_required(VERSION 3.0.0)
project(mddu_pattern_test VERSION 0.1.0)


include_directories(inc)

add_executable(main src/main.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
