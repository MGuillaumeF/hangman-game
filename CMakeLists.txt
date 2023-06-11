# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)

# Add name on project 
project(HangmanGame VERSION "0.1.0")

set(GITHUB_USERNAME "MGuillaumeF")
set(CPACK_PACKAGE_VENDOR "${GITHUB_USERNAME}")
set(CPACK_PACKAGE_CONTACT "draft@gmail.com")
set(COMPANY_NAME "Free Software Foundation, Inc")
set(LEGAL_COPYRIGHT "Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  MESSAGE("Debug mode activate test")
  # activate test on project
  enable_testing()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  MESSAGE("Compiler used is Clang")
  set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage --coverage") #-stdlib=libc++ -lc++abi
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  MESSAGE("Compiler used is g++")
  set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage -fprofile-abs-path")
  set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -ftest-coverage -lgcov --coverage")
  add_compile_options(-Wall -Wextra -pedantic)
  # if add this compile option builds failed -> -Werror
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  MESSAGE("Compiler used is Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  MESSAGE("Compiler used is Microsoft Visual Studio C++")
  set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}")

  # No flag EHsc and MD found -> set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /EHsc /MD")
  add_compile_options(/w)
endif()

add_subdirectory(./server)
if (NOT NOT_GENERATE_DB_ACCESS)
  add_subdirectory(./data-management)
endif()
