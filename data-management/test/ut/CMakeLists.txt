# Create application executable files
set(APPLICATION_NAME ${PROJECT_NAME}-data-management)
set(APPLICATION_DESCRIPTION "${APPLICATION_NAME} Server database connector")

set(APPLICATION_ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../resources/icon/data-management/data-management.ico)

set(APP_VERSION_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/versioninfo.rc)
set(APP_ICO_RESOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc)
set(APP_CONFIG_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../../versioninfo.rc.cmake.in ${APP_VERSION_RESOURCE_PATH} @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../../appicon.rc.in ${APP_ICO_RESOURCE_PATH} @ONLY)
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/../../config.hpp.in ${APP_CONFIG_FILE_PATH} @ONLY)

add_executable(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${TESTS_SRC_UT_DIR} ${CONNECTOR_MODEL_SRC_DIR} ${APP_CONFIG_FILE_PATH} ${APP_VERSION_RESOURCE_PATH} ${APP_ICO_RESOURCE_PATH})
add_test(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/../../reports/logUT.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/../../reports/reportUT.xml)
add_custom_command(TARGET ${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../../resources ${CMAKE_CURRENT_BINARY_DIR}/resources)
# add_custom_command(TARGET ${PROJECT_NAME}-data-management-unit-tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/configuration ${CMAKE_CURRENT_BINARY_DIR}/configuration)
# add_test(DataManagementUnitTests ${PROJECT_NAME}-data-management-unit-tests --show_progress=yes --log_level=all --log_format=XML --report_level=detailed --report_format=XML --log_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reports.xml --report_sink=${CMAKE_CURRENT_SOURCE_DIR}/reports/reportUT.xml)

if(WIN32)
  add_definitions("-D_WIN32_WINNT=0x0601")   
  target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${ODB_LIBRARIES} ${Boost_LIBRARIES} ${EXTERNAL_LIBS} wsock32 ws2_32)
else ()
  target_link_libraries(${PROJECT_NAME}-data-management-${SGBD_DEPENDENCY}-unit-tests ${ODB_LIBRARIES}  ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
endif()
# target_link_libraries(${PROJECT_NAME}-data-management PRIVATE odb::libodb-sqlite)
