cmake_minimum_required (VERSION 3.8)

include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/src
                     ${CMAKE_SOURCE_DIR}/src/libshared/src
                     ${CMAKE_SOURCE_DIR}/test)

include_directories (${CMAKE_INSTALL_PREFIX}/include)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)

set (test_SRCS rule.t)

add_custom_target (test ./run_all --verbose
                        DEPENDS ${test_SRCS}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/test)

foreach (src_FILE ${test_SRCS})
  add_executable (${src_FILE} "${src_FILE}.cpp" test.cpp)
  target_link_libraries (${src_FILE} clog libshared ${CLOG_LIBRARIES})
endforeach (src_FILE)

configure_file(run_all run_all COPYONLY)
configure_file(problems problems COPYONLY)
