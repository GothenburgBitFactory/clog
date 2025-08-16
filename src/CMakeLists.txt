cmake_minimum_required (VERSION 3.8)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/libshared/src
                     ${CLOG_INCLUDE_DIRS})

set (clog_SRCS clog.cpp rules.cpp Rule.cpp Rule.h)

set (libshared_SRCS
                    libshared/src/Color.cpp         libshared/src/Color.h
                    libshared/src/Composite.cpp     libshared/src/Composite.h
                    libshared/src/FS.cpp            libshared/src/FS.h
                    libshared/src/Pig.cpp           libshared/src/Pig.h
                    libshared/src/RX.cpp            libshared/src/RX.h
                    libshared/src/format.cpp        libshared/src/format.h
                    libshared/src/shared.cpp        libshared/src/shared.h
                    libshared/src/unicode.cpp       libshared/src/unicode.h
                    libshared/src/utf8.cpp          libshared/src/utf8.h
                    libshared/src/wcwidth6.cpp)

add_library (clog      STATIC ${clog_SRCS})
add_library (libshared STATIC ${libshared_SRCS})
add_executable (clog_executable clog.cpp)

target_link_libraries (clog_executable clog libshared ${CLOG_LIBRARIES})

set_property (TARGET clog_executable PROPERTY OUTPUT_NAME "clog")

install (TARGETS clog_executable DESTINATION bin)

