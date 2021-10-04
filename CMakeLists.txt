# Nous voulons un cmake "récent" pour utiliser les dernières fonctionnalités
cmake_minimum_required(VERSION 3.12)

# Notre projet est étiqueté hello
project(ApplicationExample)

set (CMAKE_CXX_STANDARD 20)

add_definitions("-Wall" "-g" "-D_WIN32_WINNT=0x0601" "-fprofile-instr-generate" "-fcoverage-mapping")

# Crée des variables avec les fichiers à compiler
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
     "server/src/*.hpp"
     "server/src/*.cpp"
)

# Recherche la dépendance externe
find_package (Boost)
if (Boost_FOUND)
  # Une fois la dépendance trouvée, nous l'incluons au projet
  include_directories(${Boost_INCLUDE_DIRS}) 
  # On indique que l'on veut un exécutable "hello" compilé à partir des fichiers décrits par les variables SRCS et HEADERS
  add_executable(ApplicationExample ${SRCS})
  if(WIN32)
    target_link_libraries(ApplicationExample ${Boost_LIBRARIES} C:/Users/miche/Desktop/boost_1_76_0/stage/lib/libboost_date_time-clangw12-mt-gd-x64-1_76.lib)
    #option(CMAKE_USE_PTHREADS_INIT "using WIN32 threads" ON)
    target_link_libraries(ApplicationExample wsock32 ws2_32)
  endif()
else ()
  # Sinon, nous affichons un message
  message(FATAL_ERROR "Boost not found")
endif ()
