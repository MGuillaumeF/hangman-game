# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)


set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")


# Add name on project 
project(HangmanGameModel VERSION "${PACKAGE_VERSION}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  MESSAGE("Debug mode activate test")
  # activate test on project
  enable_testing()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  MESSAGE("Compiler used is Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage --coverage") #-stdlib=libc++ -lc++abi
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  MESSAGE("Compiler used is g++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage -fprofile-abs-path")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -ftest-coverage -lgcov --coverage")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  MESSAGE("Compiler used is Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  MESSAGE("Compiler used is Microsoft Visual Studio C++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /EHsc /MD")
endif()


# define C++ version on standard
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
endif()

# Add all Core Model Sources files in COMMON_MODEL_SRC_DIR variable
file(GLOB_RECURSE CONNECTOR_MODEL_SRC_DIR CONFIGURE_DEPENDS
     "../src/model/*.hxx"
     "main.cpp"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Modules")

# search external dependence ODB
find_package (ODB REQUIRED COMPONENTS ${SGBD_DEPENDENCY})
find_package (Boost)
include(${ODB_USE_FILE})

if (ODB_FOUND)
  include_directories(${ODB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIRS}) 

  # Create application executable files
  add_executable(${PROJECT_NAME}-model ${CONNECTOR_MODEL_SRC_DIR})

  if(WIN32)
    add_definitions("-D_WIN32_WINNT=0x0601")
    # target_link_libraries(DataManagement wsock32 ws2_32 ${EXTERNAL_LIBS})
    target_link_libraries(${PROJECT_NAME}-model ${ODB_LIBRARIES} ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
    
  else ()
    target_link_libraries(${PROJECT_NAME}-model ${ODB_LIBRARIES}  ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
  endif()
  # target_link_libraries(${PROJECT_NAME}-data-management PRIVATE odb::libodb-sqlite)
else ()
  # If ODB not found print fatal error message
  message(FATAL_ERROR "ODB not found")
endif()

