# Nous voulons un cmake "récent" pour utiliser les dernières fonctionnalités
cmake_minimum_required(VERSION 3.12)

# Notre projet est étiqueté hello
project(HangmanGame)

set (CMAKE_CXX_STANDARD 20)

SET(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage --coverage")
SET(GCC_COVERAGE_LINK_FLAGS "-ftest-coverage --coverage")
#SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
#SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )


add_definitions("-Wall" "-g" "-D_WIN32_WINNT=0x0601")

# Crée des variables avec les fichiers à compiler
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
     "server/src/*.hpp"
     "server/src/*.cpp"
)

find_package (Threads)
# Recherche la dépendance externe
find_package (Boost)
if (Boost_FOUND)
  # Une fois la dépendance trouvée, nous l'incluons au projet
  include_directories(${Boost_INCLUDE_DIRS}) 
  # On indique que l'on veut un exécutable "hello" compilé à partir des fichiers décrits par les variables SRCS et HEADERS
  add_executable(HangmanGame ${SRCS})
  add_executable(HangmanGameTest ./server/src/common/HTTP/HttpUtils.cpp ./server/src/common/Logger/Logger.cpp ./server/test/testHttpUtils.cpp)
  if(WIN32)
    target_link_libraries(HangmanGame ${Boost_LIBRARIES} wsock32 ws2_32)
    # c:/Users/miche/Desktop/boost_1_76_0/stage/lib/libboost_date_time-clangw12-mt-gd-x64-1_76.lib
    #option(CMAKE_USE_PTHREADS_INIT "using WIN32 threads" ON)
    target_link_libraries(HangmanGameTest ${Boost_LIBRARIES} wsock32 ws2_32)
    # C:/Users/miche/Desktop/boost_1_76_0/stage/lib/libboost_date_time-clangw12-mt-gd-x64-1_76.lib 
  else ()
    add_definitions("-lboost_unit_test_framework" "--coverage")
    target_link_libraries(HangmanGame ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(HangmanGameTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  endif()
else ()
  # Sinon, nous affichons un message
  message(FATAL_ERROR "Boost not found")
endif ()
