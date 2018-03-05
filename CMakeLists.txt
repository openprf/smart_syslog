cmake_minimum_required(VERSION 3.9)
project(smart_syslog C)

set(CMAKE_C_STANDARD 99)

add_library(smart_syslog SHARED
		include/smart_syslog.h
        src/smart_syslog.c
        include/smart_syslog_utils.h
        src/smart_syslog_utils.c)

set(TARGET_NAME smart_syslog)

target_include_directories(${TARGET_NAME} PRIVATE 
			    "include/"
			    "/usr/include/"
			    )

link_directories(/usr/lib/)


target_compile_options(${TARGET_NAME} PRIVATE -O2 -Os -s -fPIC  -std=gnu99)

target_link_libraries(smart_syslog json-c)

set_target_properties( ${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin/ )

install(TARGETS smart_syslog LIBRARY DESTINATION ../bin/)