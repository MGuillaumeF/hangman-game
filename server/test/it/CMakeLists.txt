# define C++ version on standard in parent 

set(APPLICATION_NAME "${PROJECT_NAME}-Server-Integration-Test")
set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Server Integartion Tests application")

set(APPLICATION_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../resources/icon/hangman-game/hangman-game.ico)

set(APP_VERSION_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc)
set(APP_ICO_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc)
set(APP_CONFIG_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../../versioninfo.rc.cmake.in ${APP_VERSION_RESOURCE_PATH} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../../appicon.rc.in ${APP_ICO_RESOURCE_PATH} @ONLY)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/../../config.hpp.in ${APP_CONFIG_FILE_PATH} @ONLY)

add_executable(HangmanGameITTest ${TESTS_SRC_IT_DIR} ${APP_CONFIG_FILE_PATH} ${APP_VERSION_RESOURCE_PATH} ${APP_ICO_RESOURCE_PATH})
add_test(HangmanGameIntegrationTests HangmanGameITTest --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/../../reports/logIT.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/../../reports/reportIT.xml)
add_custom_command(TARGET HangmanGameITTest PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../configuration ${CMAKE_CURRENT_BINARY_DIR}/configuration)

if(WIN32)
  add_definitions("-D_WIN32_WINNT=0x0601")
  target_link_libraries(HangmanGameITTest ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
else ()
  set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  target_link_libraries(HangmanGameITTest ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
endif()