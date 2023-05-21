@echo off
REM vcpkg install libmysql --triplet=x64-windows
REM vcpkg install libodb --triplet=x64-windows
REM vcpkg install libodb-mysql --triplet=x64-windows
REM vcpkg install libodb-sqlite --triplet=x64-windows
REM vcpkg install libodb-pgsql --triplet=x64-windows
REM vcpkg install boost --triplet=x64-windows

set start_path=%CD%

cd /D %~dp0
cd model-generator
call npm install
call npm run build
copy .\dist\cpp\model\*.hxx ..\..\src\model\
copy .\dist\cpp\endpoint\*.cpp ..\..\src\endpoint\

cd /D %~dp0
cd ..
cd src/model

REM SQLITE
mkdir sqlite
copy *.hxx sqlite\
copy model_error.hpp sqlite\
cd sqlite
odb --database sqlite --generate-query --generate-schema --std c++14 -IC:/dev/vcpkg/installed/x64-windows/include/ *.hxx

cd /D %~dp0
cd ../..

call cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=SQLITE
call cmake --build bin --config Debug
cd bin/data-management/bin/Debug
call HangmanGame-data-management-sqlite-unit-tests.exe --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=reports.xml --report_sink=reportUT.xml
    

cd /D %start_path%