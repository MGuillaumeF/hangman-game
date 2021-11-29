include_directories(${Boost_INCLUDE_DIRS}) 

# Add all srcs files in SRCS variable
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
     "src/*.hpp"
     "src/*.cpp"
)

# Create application executable files
add_executable(HangmanGame ${SRCS})
add_custom_command(TARGET HangmanGame PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/configuration $<TARGET_FILE_DIR:HangmanGame>/configuration)

if(WIN32)
  target_link_libraries(HangmanGame ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
else ()
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --coverage")
  target_link_libraries(HangmanGame ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
endif()