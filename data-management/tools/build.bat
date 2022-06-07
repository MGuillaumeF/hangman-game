REM vcpkg install libmysql --triplet=x64-windows
REM vcpkg install libodb --triplet=x64-windows
REM vcpkg install libodb-mysql --triplet=x64-windows
REM vcpkg install libodb-sqlite --triplet=x64-windows
REM vcpkg install libodb-pgsql --triplet=x64-windows
REM vcpkg install boost --triplet=x64-windows

set start_path=%CD%
cd /D %~dp0
cd ..
cd src/model

REM SQLITE
mkdir sqlite
copy user.hxx sqlite\user.hxx
cd sqlite
odb --database sqlite --generate-query --generate-schema --std c++11 user.hxx

cd /D %~dp0
cd ..

cmake -S . -B bin/sqlite -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=SQLITE
cmake --build bin/sqlite --config Debug

REM PGSQL
cd src/model
mkdir pgsql
copy user.hxx pgsql\user.hxx
cd pgsql
odb --database pgsql --generate-query --generate-schema --std c++11 user.hxx

cd /D %~dp0
cd ..

cmake -S . -B bin/pgsql -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=PGSQL
cmake --build bin/pgsql --config Debug

REM MYSQL
cd src/model
mkdir mysql
copy user.hxx mysql\user.hxx
cd mysql
odb --database mysql --generate-query --generate-schema --std c++11 user.hxx

cd /D %~dp0
cd ..

cmake -S . -B bin/mysql -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DSGBD_NAME=MYSQL -DEXT_CMAKE_CXX_FLAGS="/I C:\\dev\\vcpkg\\installed\\x64-windows\\include\\mysql"
cmake --build bin/mysql --config Debug


cd /D %start_path%