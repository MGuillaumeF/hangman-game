# define C++ version on standard in parent 

set(APPLICATION_NAME "${PROJECT_NAME}-Server-Test")
set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Tests Server application")
set(APPLICATION_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../resources/icon/hangman-game/hangman-game.ico)

add_subdirectory(./ut)
add_subdirectory(./it)