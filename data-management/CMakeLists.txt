# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)

# define C++ version on standard
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# target_compile_definitions(DATABASE_MYSQL)
# add_compile_definitions(DATABASE_MYSQL)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

# Add name on project 
project(DataManagement VERSION "${PACKAGE_VERSION}")

# activate test on project
enable_testing()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  MESSAGE("Compiler used is Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage --coverage -lodb-mysql -lodb") #-stdlib=libc++ -lc++abi
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  MESSAGE("Compiler used is g++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -ftest-coverage -lgcov --coverage -lodb-mysql -lodb")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  MESSAGE("Compiler used is Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  MESSAGE("Compiler used is Microsoft Visual Studio C++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++11")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /EHsc /MD -lodb-mysql -lodb")
endif()

add_definitions("-Wall")

# Add all tests files in TESTS_SRC_UT_DIR variable
file(GLOB_RECURSE TESTS_SRC_UT_DIR CONFIGURE_DEPENDS
     "test/ut/*.hpp"
     "test/ut/*.cpp"
)

# Add all Core Model Sources files in COMMON_MODEL_SRC_DIR variable
file(GLOB_RECURSE CONNECTOR_MODEL_SRC_DIR CONFIGURE_DEPENDS
     "src/model/*-odb.cxx"
     "src/model/*-odb.hxx"
     "src/connector/*.hxx"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# search external dependence ODB
find_package (ODB REQUIRED COMPONENTS sqlite)
find_package (Boost)
include(${ODB_USE_FILE})

if (ODB_FOUND)
  include_directories(${ODB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIRS}) 

  # Create application executable files
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc.cmake.in versioninfo.rc @ONLY)
  set(APP_ICO_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")
  add_executable(${PROJECT_NAME}  src/main.cpp ${CONNECTOR_MODEL_SRC_DIR} "${CMAKE_CURRENT_BINARY_DIR}/versioninfo.rc" ${APP_ICO_RESOURCE})
  add_custom_command(TARGET DataManagement PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/configuration $<TARGET_FILE_DIR:DataManagement>/configuration)
  add_test(DataManagementUnitTests DataManagement)
  # install(TARGETS DataManagement DESTINATION DataManagement)
  # install(DIRECTORY "${PROJECT_SOURCE_DIR}/configuration/" DESTINATION "DataManagement/configuration")
  # add_executable(DataManagementTest ${TESTS_SRC_UT_DIR} ${CONNECTOR_MODEL_SRC_DIR})
  # add_custom_command(TARGET DataManagementTest PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/configuration $<TARGET_FILE_DIR:DataManagementTest>/configuration)
  # add_test(DataManagementUnitTests DataManagementTest --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=../reports/reports.xml  --report_sink=../reports/reportUT.xml)

  if(WIN32)
  add_definitions("-D_WIN32_WINNT=0x0601")
    # target_link_libraries(DataManagement wsock32 ws2_32 ${EXTERNAL_LIBS})
    target_link_libraries(DataManagement ${ODB_LIBRARIES} ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
  else ()
    target_link_libraries(DataManagement ${ODB_LIBRARIES}  ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
  endif()
  # target_link_libraries(DataManagement PRIVATE odb::libodb-sqlite)
else ()
  # If ODB not found print fatal error message
  message(FATAL_ERROR "ODB not found")
endif()

