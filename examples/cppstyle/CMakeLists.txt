cmake_minimum_required(VERSION 3.9)
project(cppstyle)

set(CMAKE_CXX_STANDARD 11)
link_directories(../../build/)

add_executable(cppstyle main.cpp
        ../../include/SmartSyslogger.h
        CppStyleLog.cpp
        CppStyleLog.h)

target_compile_options(cppstyle PRIVATE -O2 -Os -s -std=c++11 -fpermissive)

target_include_directories(cppstyle PRIVATE ../../include)

target_link_libraries(cppstyle PRIVATE smart_syslog)
