# define C++ version on standard
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (SGBD_NAME STREQUAL "SQLITE")
  set(SGBD_DEPENDENCY "sqlite")
  elseif (SGBD_NAME STREQUAL "MYSQL")
  set(SGBD_DEPENDENCY "mysql")
  elseif (SGBD_NAME STREQUAL "PGSQL")
  set(SGBD_DEPENDENCY "pgsql")
  elseif (SGBD_NAME STREQUAL "MSSQL")
  set(SGBD_DEPENDENCY "mssql")
  elseif (SGBD_NAME STREQUAL "ORACLE")
  set(SGBD_DEPENDENCY "oracle")
  else ()
    message("SGBD not found, default SGBD sqlite")
    set(SGBD_NAME "SQLITE")
    set(SGBD_DEPENDENCY "sqlite")
endif()

set(MODEL_SRCS "src/model/${SGBD_DEPENDENCY}/*-odb.cxx" "src/model/${SGBD_DEPENDENCY}/*-odb.hxx")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif()

add_definitions("-Wall")

# Create application executable files
set(APPLICATION_NAME ${PROJECT_NAME}-data-management)
set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Server database connector")
configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.hpp @ONLY)
set(APPLICATION_ICON_PATH "resources/icon/data-management/data-management.ico")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../appicon.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../versioninfo.rc.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc @ONLY)
set(APP_ICO_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

# Add all tests files in TESTS_SRC_UT_DIR variable
file(GLOB_RECURSE TESTS_SRC_UT_DIR CONFIGURE_DEPENDS
     "test/ut/*.hpp"
     "test/ut/*.cpp"
)

# Add all Core Model Sources files in COMMON_MODEL_SRC_DIR variable
file(GLOB_RECURSE CONNECTOR_MODEL_SRC_DIR CONFIGURE_DEPENDS
     ${MODEL_SRCS}
     "src/model/*.hpp"
     "src/endpoint/*.hpp"
     "src/endpoint/*.cpp"
     "src/connector/*.hxx"
     "src/serializer/*.hpp"
     "src/TCP/*.hpp"
     "src/TCP/*.cpp"
)

file(GLOB_RECURSE TEST_CLIENT_TCP_SRC_UT_DIR CONFIGURE_DEPENDS
     "src/ext/*.cpp"
     "src/TCP/Client.hpp"
     "src/TCP/Client.cpp"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

# search external dependence ODB
find_package (ODB REQUIRED COMPONENTS ${SGBD_DEPENDENCY})
find_package (Boost)
include(${ODB_USE_FILE})

if (ODB_FOUND)
  include_directories(${ODB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIRS}) 

  add_executable(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}  src/main.cpp src/config.hpp ${CONNECTOR_MODEL_SRC_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc" ${APP_ICO_RESOURCE})
  add_custom_command(TARGET ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configuration ${CMAKE_CURRENT_BINARY_DIR}/configuration)
  add_custom_command(TARGET ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/database-order ${CMAKE_CURRENT_BINARY_DIR}/resources/database-order)

  install(TARGETS ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} DESTINATION ${PROJECT_NAME}/)
  # install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/configuration/" DESTINATION "${PROJECT_NAME}/configuration")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${TESTS_SRC_UT_DIR} ${CONNECTOR_MODEL_SRC_DIR}  src/config.hpp "${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc" ${APP_ICO_RESOURCE})
    add_test(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/logUT.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reportUT.xml)
  endif()
  add_executable(${PROJECT_NAME}-data-management-tcp-client ${TEST_CLIENT_TCP_SRC_UT_DIR}  src/config.hpp)
  add_custom_command(TARGET ${PROJECT_NAME}-data-management-tcp-client PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources/database-order ${CMAKE_CURRENT_BINARY_DIR}/resources/database-order)
  # add_custom_command(TARGET ${PROJECT_NAME}-data-management-unit-tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configuration ${CMAKE_CURRENT_BINARY_DIR}/configuration)
  # add_test(DataManagementUnitTests ${PROJECT_NAME}-data-management-unit-tests --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reports.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reportUT.xml)

  if(WIN32)
    add_definitions("-D_WIN32_WINNT=0x0601")
    # target_link_libraries(DataManagement wsock32 ws2_32 ${EXTERNAL_LIBS})
    target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} ${ODB_LIBRARIES} ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
    
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${ODB_LIBRARIES} ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
    endif()
  else ()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${ODB_LIBRARIES}  ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
    endif()
    target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} ${ODB_LIBRARIES}  ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
  endif()
  # target_link_libraries(${PROJECT_NAME}-data-management PRIVATE odb::libodb-sqlite)
else ()
  # If ODB not found print fatal error message
  message(FATAL_ERROR "ODB not found")
endif()

