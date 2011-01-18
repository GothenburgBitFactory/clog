cmake_minimum_required (VERSION 2.8)

project (clog)
set (PROJECT_VERSION "0.9.0")

set (PACKAGE "${PROJECT_NAME}")
set (VERSION "${PROJECT_VERSION}")
set (PACKAGE_BUGREPORT "support@yootabory.org")
set (PACKAGE_NAME "${PACKAGE}")
set (PACKAGE_TARNAME "${PACKAGE}")
set (PACKAGE_VERSION "${VERSION}")
set (PACKAGE_STRING "${PACKAGE} ${VERSION}")

add_subdirectory (src)
add_subdirectory (test EXCLUDE_FROM_ALL)

set (doc_FILES ChangeLog README INSTALL AUTHORS COPYING)
foreach (doc_FILE ${doc_FILES})
  install (FILES ${doc_FILE}  DESTINATION share/doc/clog)
endforeach (doc_FILE)
