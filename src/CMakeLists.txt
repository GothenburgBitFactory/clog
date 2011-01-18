include_directories (${CMAKE_SOURCE_DIR}/src
                     ${CLOG_INCLUDE_DIRS})

set (clog_SRCS Date.cpp Date.h
               File.cpp File.h
               Nibbler.cpp Nibbler.h
               Path.cpp Path.h
               rx.cpp rx.h
               text.cpp text.h
               util.cpp util.h
               defs.h)

add_executable(clog clog.cpp)

install (TARGETS clog DESTINATION bin)

set (CMAKE_BUILD_TYPE debug)
set (CMAKE_C_FLAGS_DEBUG "-ggdb3")
set (CMAKE_C_FLAGS_RELEASE "-O3")

