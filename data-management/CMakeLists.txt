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
  add_subdirectory(./src ./app)
  if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_subdirectory(./test)
    endif()
  else ()
    add_subdirectory(./test)
  endif()
else ()
# If ODB not found print fatal error message
message(FATAL_ERROR "ODB not found")
endif()