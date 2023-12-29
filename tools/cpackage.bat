set BUILD_TYPE=Release
cd %~dp0..
call cmake -S . -B bin -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=SQLITE 
REM -DNOT_GENERATE_DB_ACCESS=true
call cmake --build bin --config %BUILD_TYPE%
cd bin 
call "C:\Program Files\CMake\bin\cpack" -G "ZIP;NSIS64"
cd %~dp0