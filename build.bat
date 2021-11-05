@echo off

set DCMAKE_GENERATOR="MinGW Makefiles"
set DCMAKE_CXX_COMPILER=clang++.exe
set DCMAKE_C_COMPILER=clang.exe
set DCMAKE_MAKE_PROGRAM=mingw32-make.exe

set DCMAKE_BUILD_TYPE=Debug
set DESTINATION_DIRECTORY=./server/bin

echo. GENERATE TARGET
cmake -G %DCMAKE_GENERATOR% -DCMAKE_CXX_COMPILER=%DCMAKE_CXX_COMPILER% -DCMAKE_C_COMPILER=%DCMAKE_C_COMPILER% -DCMAKE_MAKE_PROGRAM=%DCMAKE_MAKE_PROGRAM% -DCMAKE_BUILD_TYPE=%DCMAKE_BUILD_TYPE% -B %DESTINATION_DIRECTORY%
echo.
echo. BUILD TARGET
cmake --build %DESTINATION_DIRECTORY%

echo. END
pause