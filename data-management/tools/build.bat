REM vcpkg install libodb --triplet=x64-windows
REM vcpkg install libodb-mysql --triplet=x64-windows
REM vcpkg install libodb-sqlite --triplet=x64-windows
REM vcpkg install libodb-pgsql --triplet=x64-windows
REM vcpkg install boost --triplet=x64-windows

set start_path=%CD%
cd /D %~dp0
cd ..
cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build bin --config Debug

cd /D %start_path%