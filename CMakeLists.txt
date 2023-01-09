# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)


set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")


# Add name on project 
project(HangmanGame VERSION "${PACKAGE_VERSION}")

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
  add_compile_options(/w)
endif()

add_definitions("-Wall")

add_subdirectory(./server ./server/bin)
if (NOT NOT_GENERATE_DB_ACCESS)
  add_subdirectory(./data-management ./data-management/bin)
endif()
