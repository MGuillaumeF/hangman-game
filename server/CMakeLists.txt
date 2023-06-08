# define C++ version on standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

  # Create application executable files
  set(APPLICATION_NAME "${PROJECT_NAME}")
  set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Server application")
  configure_file (config.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.hpp @ONLY)
  set(APPLICATION_ICON_PATH "resources/icon/hangman-game/hangman-game.ico")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../appicon.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc @ONLY)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../versioninfo.rc.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc @ONLY)
  set(APP_ICO_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")

# Add all srcs files in SRCS variable
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
     "src/*.hpp"
     "src/*.cpp"
)

# Add all tests files in TESTS_SRC_UT_DIR variable
file(GLOB_RECURSE TESTS_SRC_UT_DIR CONFIGURE_DEPENDS
     "test/ut/*.hpp"
     "test/ut/*.cpp"
)

# Add all tests files in TESTS_SRC_IT_DIR variable
file(GLOB_RECURSE TESTS_SRC_IT_DIR CONFIGURE_DEPENDS
     "test/it/*.hpp"
     "test/it/*.cpp"
)

# Add all API files in API_SRC_DIR variable
file(GLOB_RECURSE API_SRC_DIR CONFIGURE_DEPENDS
     "src/api/*.hpp"
     "src/api/*.cpp"
)

# Add all HTTP files in HTTP_SRC_DIR variable
file(GLOB_RECURSE HTTP_SRC_DIR CONFIGURE_DEPENDS
     "src/common/HTTP/*.hpp"
     "src/common/HTTP/*.cpp"
)

# Add all Logger files in LOGGER_SRC_DIR variable
file(GLOB_RECURSE LOGGER_SRC_DIR CONFIGURE_DEPENDS
     "src/common/Logger/*.hpp"
     "src/common/Logger/*.cpp"
)

find_package (Threads)
# search external dependence BOOST
find_package (Boost)
if (Boost_FOUND)
  # add boost includes
  include_directories(${Boost_INCLUDE_DIRS}) 

  add_executable(${PROJECT_NAME} ${SRCS} "${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc" ${APP_ICO_RESOURCE})
  if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_custom_command(TARGET HangmanGame PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configuration ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/configuration)
  else ()
    add_custom_command(TARGET HangmanGame PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configuration ${CMAKE_CURRENT_BINARY_DIR}/configuration)
  endif()
  install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/configuration/" DESTINATION "${PROJECT_NAME}/configuration")
  install(DIRECTORY "${PROJECT_SOURCE_DIR}/client/build/" DESTINATION "${PROJECT_NAME}/")
  
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(HangmanGameTest ${TESTS_SRC_UT_DIR} ${HTTP_SRC_DIR} ${LOGGER_SRC_DIR} ${API_SRC_DIR}  src/config.hpp )
    add_test(HangmanGameUnitTests HangmanGameTest --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/logUT.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reportUT.xml) 
    add_executable(HangmanGameITTest ${TESTS_SRC_IT_DIR} src/config.hpp )
    add_test(HangmanGameIntegrationTests HangmanGameITTest --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/logIT.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reportIT.xml)
  endif()

  if(WIN32)
  add_definitions("-D_WIN32_WINNT=0x0601")
    target_link_libraries(HangmanGame ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_link_libraries(HangmanGameTest ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
      target_link_libraries(HangmanGameITTest ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
    endif()
  else ()
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    target_link_libraries(HangmanGame ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_link_libraries(HangmanGameTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
      target_link_libraries(HangmanGameITTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    endif()
  endif()
else ()
  # If boost not found print fatal error message
  message(FATAL_ERROR "Boost not found")
endif ()

# CPACK DEFINITION
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "HangmanGame")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Hangman game, Backend server")
set(CPACK_PACKAGE_VENDOR "MGuillaumeF")
set(CPACK_PACKAGE_CONTACT "draft@gmail.com")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/docs/README.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/docs/Copyright.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full UNIX paths properly.
  # Make sure there is at least one set of four backlashes.
  # set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\HangmanGame.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Hangman Game")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.github.com/MGuillaumeF")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.github.com/MGuillaumeF")
  set(CPACK_NSIS_CONTACT "draft@gmail.com")
  set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_STRIP_FILES "bin/HangmanGame")
  set(CPACK_SOURCE_STRIP_FILES "")
endif()
set(CPACK_PACKAGE_EXECUTABLES "HangmanGame" "Hangman Game")
include(CPack)
