# define C++ version on standard in parent 

# Create application executable files
set(APPLICATION_NAME ${PROJECT_NAME}-data-management)
set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Server database connector")

set(APPLICATION_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../resources/icon/data-management/data-management.ico)

set(APP_VERSION_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc)
set(APP_ICO_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc)
set(APP_CONFIG_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../versioninfo.rc.cmake.in ${APP_VERSION_RESOURCE_PATH} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../appicon.rc.in ${APP_ICO_RESOURCE_PATH} @ONLY)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/../config.hpp.in ${APP_CONFIG_FILE_PATH} @ONLY)

include_directories(${ODB_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIRS}) 

add_executable(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}  main.cpp ${CONNECTOR_MODEL_SRC_DIR} ${APP_VERSION_RESOURCE_PATH} ${APP_ICO_RESOURCE_PATH} ${APP_CONFIG_FILE_PATH})
add_custom_command(TARGET ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../configuration ${CMAKE_CURRENT_BINARY_DIR}/configuration)
add_custom_command(TARGET ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources/database-order ${CMAKE_CURRENT_BINARY_DIR}/resources/database-order)

install(TARGETS ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} DESTINATION ${PROJECT_NAME}/)
# install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/configuration/" DESTINATION "${PROJECT_NAME}/configuration")

add_executable(${PROJECT_NAME}-data-management-tcp-client ${TEST_CLIENT_TCP_SRC_UT_DIR} ${APP_VERSION_RESOURCE_PATH} ${APP_ICO_RESOURCE_PATH} ${APP_CONFIG_FILE_PATH})
add_custom_command(TARGET ${PROJECT_NAME}-data-management-tcp-client PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../resources/database-order ${CMAKE_CURRENT_BINARY_DIR}/resources/database-order)

if(WIN32)
  add_definitions("-D_WIN32_WINNT=0x0601")
  # target_link_libraries(DataManagement wsock32 ws2_32 ${EXTERNAL_LIBS})
  target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} ${ODB_LIBRARIES} ${Boost_LIBRARIES} wsock32 ws2_32 ${EXTERNAL_LIBS})
else ()
  target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY} ${ODB_LIBRARIES}  ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
endif()
# target_link_libraries(${PROJECT_NAME}-data-management PRIVATE odb::libodb-sqlite)