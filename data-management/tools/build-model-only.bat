set start_path=%CD%

cd /D %~dp0

cmake -S . -B bin -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=SQLITE
cmake --build bin --config Debug

cd /D %start_path%