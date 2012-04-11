cmake_minimum_required (VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/test
                     ${CLOG_INCLUDE_DIRS})

set (test_SRCS color.t nibbler.t)

add_custom_target (test ./run_all DEPENDS ${test_SRCS}
                                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

add_custom_target (build_tests DEPENDS ${test_SRCS}
                               WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

foreach (src_FILE ${test_SRCS})
  add_executable (${src_FILE} "${src_FILE}.cpp"
                              ../src/Nibbler.cpp
                              ../src/RX.cpp
                              ../src/Color.cpp
                              ../src/text.cpp
                              test.cpp)
endforeach (src_FILE)

