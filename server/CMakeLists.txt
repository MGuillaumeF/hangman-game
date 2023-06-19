# define C++ version on standard
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++20")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -fmodule-header")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

# Add all srcs files in SRCS variable
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
     "src/*.hpp"
     "src/*.cpp"
)

# Add all tests files in TESTS_SRC_UT_DIR variable
file(GLOB_RECURSE TESTS_SRC_UT_DIR CONFIGURE_DEPENDS
     "test/ut/*.hpp"
     "test/ut/*.cpp"
)

# Add all tests files in TESTS_SRC_IT_DIR variable
file(GLOB_RECURSE TESTS_SRC_IT_DIR CONFIGURE_DEPENDS
     "test/it/*.hpp"
     "test/it/*.cpp"
)

# Add all API files in API_SRC_DIR variable
file(GLOB_RECURSE API_SRC_DIR CONFIGURE_DEPENDS
     "src/api/*.hpp"
     "src/api/*.cpp"
)

# Add all HTTP files in HTTP_SRC_DIR variable
file(GLOB_RECURSE HTTP_SRC_DIR CONFIGURE_DEPENDS
     "src/common/HTTP/*.hpp"
     "src/common/HTTP/*.cpp"
)

# Add all Logger files in LOGGER_SRC_DIR variable
file(GLOB_RECURSE LOGGER_SRC_DIR CONFIGURE_DEPENDS
     "src/common/Logger/*.hpp"
     "src/common/Logger/*.cpp"
)

find_package (Threads)
# search external dependence BOOST
find_package (Boost)
if (Boost_FOUND)
  # add boost includes
  include_directories(${Boost_INCLUDE_DIRS})
  add_subdirectory(./src ./app)
  if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_subdirectory(./test)
    endif()
  else ()
    add_subdirectory(./test)
  endif()
else ()
  # If boost not found print fatal error message
  message(FATAL_ERROR "Boost not found")
endif ()
