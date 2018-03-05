cmake_minimum_required(VERSION 3.9)
project(cstyle C)

set(CMAKE_C_STANDARD 99)
link_directories(../../build/)

add_executable(cstyle_example main.c cStyleLog.h cStyleLog.c ../../include/smart_syslog.h)

include_directories(../../include)

target_link_libraries(cstyle_example PRIVATE smart_syslog)