set start_path=%CD%
set BUILD_TYPE=Release

cd /D %~dp0

cmake -S . -B bin -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=SQLITE
cmake --build bin --config %BUILD_TYPE%

cd /D %start_path%