set(VCPKG_TARGET_ARCHITECTURE arm64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)

set(VCPKG_CMAKE_SYSTEM_NAME Linux)

set(VCPKG_C_FLAGS "${VCPKG_C_FLAGS} ${VCPKG_C_FLAGS_RELEASE}")
set(VCPKG_CXX_FLAGS "-std=c++14 ${VCPKG_CXX_FLAGS} ${VCPKG_CXX_FLAGS_RELEASE}")
