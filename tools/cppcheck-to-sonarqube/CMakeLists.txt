# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)

# version setting for main app
set (cppcheck-to-sonarqube_VERSION_MAJOR 1)
set (cppcheck-to-sonarqube_VERSION_MINOR 0) 

# define C++ version on standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Add name on project 
project(cppcheck-to-sonarqube)

# activate test on project
enable_testing()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  MESSAGE("Compiler used is Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -ftest-coverage --coverage") #-stdlib=libc++ -lc++abi
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  MESSAGE("Compiler used is g++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a -g -O0 -fprofile-arcs -ftest-coverage --coverage")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -ftest-coverage -lgcov --coverage")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  MESSAGE("Compiler used is Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  MESSAGE("Compiler used is Microsoft Visual Studio C++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++20")
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} /EHsc /MD")
endif()

add_definitions("-Wall")

# Add all tests files in TESTS_SRC_UT_DIR variable
file(GLOB_RECURSE TESTS_SRC_UT_DIR CONFIGURE_DEPENDS
     "test/ut/*.hpp"
     "test/ut/*.cpp"
)

# Add all Report files in REPORT_SRC_DIR variable
file(GLOB_RECURSE REPORT_SRC_DIR CONFIGURE_DEPENDS
     "src/report/*.hpp"
     "src/report/*.cpp"
)

find_package (Threads)
# search external dependence BOOST
find_package (Boost)
if (Boost_FOUND)
  # add boost includes
  include_directories(${Boost_INCLUDE_DIRS}) 
  # Create application executable files
  add_executable(cppcheck-to-sonarqube ${REPORT_SRC_DIR} "src/main.cpp")
  install(TARGETS cppcheck-to-sonarqube DESTINATION cppcheck-to-sonarqube)
  add_executable(cppcheck-to-sonarqubeTest ${TESTS_SRC_UT_DIR} ${REPORT_SRC_DIR})
  add_test(cppcheck-to-sonarqubeUnitTests cppcheck-to-sonarqubeTest --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=../reports/reports.xml  --report_sink=../reports/reportUT.xml) 
  
  if(WIN32)
  add_definitions("-D_WIN32_WINNT=0x0601")
    target_link_libraries(cppcheck-to-sonarqube ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
    target_link_libraries(cppcheck-to-sonarqubeTest ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
  else ()
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    target_link_libraries(cppcheck-to-sonarqube ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(cppcheck-to-sonarqubeTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  endif()
else ()
  # If boost not found print fatal error message
  message(FATAL_ERROR "Boost not found")
endif ()

# CPACK DEFINITION
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "cppcheck-to-sonarqube")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "cppcheck report to sonarqube report")
set(CPACK_PACKAGE_VENDOR "MGuillaumeF")
set(CPACK_PACKAGE_CONTACT "draft@gmail.com")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/docs/README.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/docs/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}.${CMake_VERSION_PATCH}")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full UNIX paths properly.
  # Make sure there is at least one set of four backlashes.
  # set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\cppcheck-to-sonarqube.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} cppcheck to sonarqube")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.github.com/MGuillaumeF")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.github.com/MGuillaumeF")
  set(CPACK_NSIS_CONTACT "draft@gmail.com")
  set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_STRIP_FILES "bin/cppcheck-to-sonarqube")
  set(CPACK_SOURCE_STRIP_FILES "")
endif()
set(CPACK_PACKAGE_EXECUTABLES "cppcheck-to-sonarqube" "cppcheck report to sonarqube report")
include(CPack)
