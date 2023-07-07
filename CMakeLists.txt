# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)

# Add name on project 
project(HangmanGame VERSION "0.1.0")

set(GITHUB_USERNAME "MGuillaumeF")
set(CPACK_PACKAGE_VENDOR "${GITHUB_USERNAME}")
set(CPACK_PACKAGE_CONTACT "mguillaumef@gmail.com")
set(COMPANY_NAME "")
set(LEGAL_COPYRIGHT "")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  MESSAGE("Debug mode activate test")
  # activate test on project
  enable_testing()
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  MESSAGE("Compiler used is Clang")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage -lgcov --coverage")
  else ()
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  MESSAGE("Compiler used is g++")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -pedantic -Wformat -Wformat-security -Winit-self -Wparentheses -Wsequence-point -Wreturn-type -Wunknown-pragmas -Wfloat-equal -Wtype-limits -Woverlength-strings -Waddress")
  SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -ftest-coverage -lgcov --coverage")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage --coverage")
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage -lgcov --coverage")
  else ()
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O")
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  MESSAGE("Compiler used is Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  MESSAGE("Compiler used is Microsoft Visual Studio C++")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20 /Wall /Zi /D_CRT_NONSTDC_NO_DEPRECATE /D_CRT_SECURE_NP_DEPRECATE /DNOMINMAX")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}") 
  #/EHsc /MD
endif()

  # No flag EHsc and MD found -> set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /EHsc /MD")
  add_compile_options(/w)
endif()

add_subdirectory(./server)
if (NOT NOT_GENERATE_DB_ACCESS)
  add_subdirectory(./data-management)
endif()
