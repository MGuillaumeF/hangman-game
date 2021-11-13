# minimum CMake version (minimum feature)
cmake_minimum_required(VERSION 3.12)

# version setting for main app
set (HangmanGame_VERSION_MAJOR 1)
set (HangmanGame_VERSION_MINOR 0) 

# define C++ version on standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Add name on project 
project(HangmanGame)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  MESSAGE("Compiler used is Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20 -g -O0 -fprofile-arcs -ftest-coverage --coverage")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  MESSAGE("Compiler used is g++")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a -g -O0 -fprofile-arcs -ftest-coverage --coverage")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  # using Intel C++
  MESSAGE("Compiler used is Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
  MESSAGE("Compiler used is Microsoft Visual Studio C++")
    SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

add_definitions("-Wall" "-g" "-D_WIN32_WINNT=0x0601")

# Add all srcs files in SRCS variable
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
     "server/src/*.hpp"
     "server/src/*.cpp"
)

# Add all tests files in TESTS_SRC_UT_DIR variable
file(GLOB_RECURSE TESTS_SRC_UT_DIR CONFIGURE_DEPENDS
     "server/test/ut/*.hpp"
     "server/test/ut/*.cpp"
)


# Add all tests files in TESTS_SRC_IT_DIR variable
file(GLOB_RECURSE TESTS_SRC_IT_DIR CONFIGURE_DEPENDS
     "server/test/it/*.hpp"
     "server/test/it/*.cpp"
)

# Add all HTTP files in HTTP_SRC_DIR variable
file(GLOB_RECURSE HTTP_SRC_DIR CONFIGURE_DEPENDS
     "server/src/common/HTTP/*.hpp"
     "server/src/common/HTTP/*.cpp"
)

# Add all Logger files in LOGGER_SRC_DIR variable
file(GLOB_RECURSE LOGGER_SRC_DIR CONFIGURE_DEPENDS
     "server/src/common/Logger/*.hpp"
     "server/src/common/Logger/*.cpp"
)

find_package (Threads)
# search external dependence BOOST
find_package (Boost)
if (Boost_FOUND)
  # add boost includes
  include_directories(${Boost_INCLUDE_DIRS}) 
  # Create application executable files
  add_executable(HangmanGame ${SRCS})
  add_executable(HangmanGameTest ${TESTS_SRC_UT_DIR} ${HTTP_SRC_DIR} ${LOGGER_SRC_DIR})
  add_executable(HangmanGameITTest ${TESTS_SRC_IT_DIR})
  if(WIN32)
    target_link_libraries(HangmanGame ${Boost_LIBRARIES} wsock32 ws2_32 
      C:/Users/miche/Desktop/boost_1_76_0/stage/lib/libboost_date_time-clangw13-mt-gd-x64-1_76.lib
      )
    #option(CMAKE_USE_PTHREADS_INIT "using WIN32 threads" ON)
    target_link_libraries(HangmanGameTest ${Boost_LIBRARIES} wsock32 ws2_32 
      C:/Users/miche/Desktop/boost_1_76_0/stage/lib/libboost_date_time-clangw13-mt-gd-x64-1_76.lib
    )
    target_link_libraries(HangmanGameITTest ${Boost_LIBRARIES} wsock32 ws2_32 
      C:/Users/miche/Desktop/boost_1_76_0/stage/lib/libboost_date_time-clangw13-mt-gd-x64-1_76.lib
    )
  else ()
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
    target_link_libraries(HangmanGame ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
    target_link_libraries(HangmanGameTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(HangmanGameITTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  endif()
else ()
  # If boost not found print fatal error message
  message(FATAL_ERROR "Boost not found")
endif ()
