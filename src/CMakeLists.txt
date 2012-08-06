cmake_minimum_required(VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}
                     ${CLOG_INCLUDE_DIRS})
add_executable(clog clog.cpp
                    Nibbler.cpp Nibbler.h
                    RX.cpp RX.h
                    Rule.cpp Rule.h
                    Color.cpp Color.h
                    text.cpp text.h)

install (TARGETS clog DESTINATION bin)

#set (CMAKE_BUILD_TYPE debug)
#set (CMAKE_C_FLAGS_DEBUG "-ggdb3")
#set (CMAKE_C_FLAGS_RELEASE "-O3")



