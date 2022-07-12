@echo off
set START_PATH=%CD%
cd /D %~dp0
cd ..
set PROJECT_PATH=%CD%
cd bin/server/bin/Debug
call HangmanGameTest.exe --list_content=DOT > "%PROJECT_PATH%/server/reports/content.dot" 2>&1
call dot "%PROJECT_PATH%/server/reports/content.dot" -Tsvg -o "%PROJECT_PATH%/server/reports/server-ut-tests.svg"
cd %PROJECT_PATH%/bin/data-management/bin/Debug
call HangmanGame-data-management-sqlite-unit-tests.exe --list_content=DOT > "%PROJECT_PATH%/data-management/reports/content.dot" 2>&1
call dot "%PROJECT_PATH%/data-management/reports/content.dot" -Tsvg -o "%PROJECT_PATH%/data-management/reports/data-management-ut-tests.svg"
cd %PROJECT_PATH%
call doxygen docg.conf

cd /D %START_PATH%