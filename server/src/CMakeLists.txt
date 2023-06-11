# define C++ version on standard in parent 

# Create application executable files
set(APPLICATION_NAME ${PROJECT_NAME})
set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Server application")

set(APPLICATION_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../resources/icon/hangman-game/hangman-game.ico)

set(APP_VERSION_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc)
set(APP_ICO_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc)
set(APP_CONFIG_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)
set(APP_RUN_CONFIG_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../configuration)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../versioninfo.rc.cmake.in ${APP_VERSION_RESOURCE_PATH} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../appicon.rc.in ${APP_ICO_RESOURCE_PATH} @ONLY)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/../config.hpp.in ${APP_CONFIG_FILE_PATH} @ONLY)

add_executable(${PROJECT_NAME} ${SRCS} ${APP_VERSION_RESOURCE_PATH} ${APP_ICO_RESOURCE_PATH})
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_custom_command(TARGET ${APPLICATION_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${APP_RUN_CONFIG_DIR_PATH} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/configuration)
else ()
  add_custom_command(TARGET ${APPLICATION_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${APP_RUN_CONFIG_DIR_PATH} ${CMAKE_CURRENT_BINARY_DIR}/configuration)
endif()
install(TARGETS ${APPLICATION_NAME} DESTINATION ${APPLICATION_NAME})
install(DIRECTORY "${APP_RUN_CONFIG_DIR_PATH}/" DESTINATION "${APPLICATION_NAME}/configuration")
install(DIRECTORY "${PROJECT_SOURCE_DIR}/client/build/" DESTINATION "${APPLICATION_NAME}/")

if(WIN32)
add_definitions("-D_WIN32_WINNT=0x0601")
  target_link_libraries(${APPLICATION_NAME} ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
else ()
  SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  target_link_libraries(${APPLICATION_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
endif()

# CPACK DEFINITION
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME ${APPLICATION_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Hangman game, Backend server")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/docs/README.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/docs/Copyright.txt")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${APPLICATION_NAME}-${CMAKE_PROJECT_VERSION}")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full UNIX paths properly.
  # Make sure there is at least one set of four backlashes.
  # set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\HangmanGame.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} Hangman Game")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.github.com/${GITHUB_USERNAME}")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.github.com/${GITHUB_USERNAME}")
  set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
  set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_STRIP_FILES "bin/HangmanGame")
  set(CPACK_SOURCE_STRIP_FILES "")
endif()
set(CPACK_PACKAGE_EXECUTABLES "HangmanGame" "Hangman Game")
include(CPack)
