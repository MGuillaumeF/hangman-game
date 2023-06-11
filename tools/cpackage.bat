cd %~dp0..
call cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=SQLITE
call cmake --build bin --config Debug
REM "C:\Program Files\CMake\bin\cpack" -G "ZIP;NSIS64"
cd %~dp0