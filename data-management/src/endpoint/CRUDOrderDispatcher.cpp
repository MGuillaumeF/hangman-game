#include "./CRUDOrderDispatcher.hpp"

#if defined(DATABASE_MYSQL)
#include "../model/mysql/chat-odb.hxx"
#include "../model/mysql/chat.hxx"
#include "../model/mysql/dictionary-odb.hxx"
#include "../model/mysql/dictionary.hxx"
#include "../model/mysql/group-odb.hxx"
#include "../model/mysql/group.hxx"
#include "../model/mysql/message-odb.hxx"
#include "../model/mysql/message.hxx"
#include "../model/mysql/party-odb.hxx"
#include "../model/mysql/party.hxx"
#include "../model/mysql/root_model_object-odb.hxx"
#include "../model/mysql/root_model_object.hxx"
#include "../model/mysql/team-odb.hxx"
#include "../model/mysql/team.hxx"
#include "../model/mysql/user-odb.hxx"
#include "../model/mysql/user.hxx"
#include "../model/mysql/word-odb.hxx"
#include "../model/mysql/word.hxx"

#elif defined(DATABASE_SQLITE)
#include "../model/sqlite/chat-odb.hxx"
#include "../model/sqlite/chat.hxx"
#include "../model/sqlite/dictionary-odb.hxx"
#include "../model/sqlite/dictionary.hxx"
#include "../model/sqlite/group-odb.hxx"
#include "../model/sqlite/group.hxx"
#include "../model/sqlite/message-odb.hxx"
#include "../model/sqlite/message.hxx"
#include "../model/sqlite/party-odb.hxx"
#include "../model/sqlite/party.hxx"
#include "../model/sqlite/root_model_object-odb.hxx"
#include "../model/sqlite/root_model_object.hxx"
#include "../model/sqlite/team-odb.hxx"
#include "../model/sqlite/team.hxx"
#include "../model/sqlite/user-odb.hxx"
#include "../model/sqlite/user.hxx"
#include "../model/sqlite/word-odb.hxx"
#include "../model/sqlite/word.hxx"

#elif defined(DATABASE_PGSQL)
#include "../model/pgsql/chat-odb.hxx"
#include "../model/pgsql/chat.hxx"
#include "../model/pgsql/dictionary-odb.hxx"
#include "../model/pgsql/dictionary.hxx"
#include "../model/pgsql/group-odb.hxx"
#include "../model/pgsql/group.hxx"
#include "../model/pgsql/message-odb.hxx"
#include "../model/pgsql/message.hxx"
#include "../model/pgsql/party-odb.hxx"
#include "../model/pgsql/party.hxx"
#include "../model/pgsql/root_model_object-odb.hxx"
#include "../model/pgsql/root_model_object.hxx"
#include "../model/pgsql/team-odb.hxx"
#include "../model/pgsql/team.hxx"
#include "../model/pgsql/user-odb.hxx"
#include "../model/pgsql/user.hxx"
#include "../model/pgsql/word-odb.hxx"
#include "../model/pgsql/word.hxx"

#elif defined(DATABASE_ORACLE)
#include "../model/oracle/chat-odb.hxx"
#include "../model/oracle/chat.hxx"
#include "../model/oracle/dictionary-odb.hxx"
#include "../model/oracle/dictionary.hxx"
#include "../model/oracle/group-odb.hxx"
#include "../model/oracle/group.hxx"
#include "../model/oracle/message-odb.hxx"
#include "../model/oracle/message.hxx"
#include "../model/oracle/party-odb.hxx"
#include "../model/oracle/party.hxx"
#include "../model/oracle/root_model_object-odb.hxx"
#include "../model/oracle/root_model_object.hxx"
#include "../model/oracle/team-odb.hxx"
#include "../model/oracle/team.hxx"
#include "../model/oracle/user-odb.hxx"
#include "../model/oracle/user.hxx"
#include "../model/oracle/word-odb.hxx"
#include "../model/oracle/word.hxx"

#elif defined(DATABASE_MSSQL)
#include "../model/mssql/chat-odb.hxx"
#include "../model/mssql/chat.hxx"
#include "../model/mssql/dictionary-odb.hxx"
#include "../model/mssql/dictionary.hxx"
#include "../model/mssql/group-odb.hxx"
#include "../model/mssql/group.hxx"
#include "../model/mssql/message-odb.hxx"
#include "../model/mssql/message.hxx"
#include "../model/mssql/party-odb.hxx"
#include "../model/mssql/party.hxx"
#include "../model/mssql/root_model_object-odb.hxx"
#include "../model/mssql/root_model_object.hxx"
#include "../model/mssql/team-odb.hxx"
#include "../model/mssql/team.hxx"
#include "../model/mssql/user-odb.hxx"
#include "../model/mssql/user.hxx"
#include "../model/mssql/word-odb.hxx"
#include "../model/mssql/word.hxx"

#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

#include <iostream>

/**
 * @brief Route to dispatch crud order by object type
 *
 * @param objectType The object requested
 * @param properties The properties part of request
 * @param data The data part of request
 * @return The result of request process
 */
boost::property_tree::ptree
CRUDOrderDispatcher::route(const std::string &objectType,
                           const boost::property_tree::ptree &properties,
                           const boost::property_tree::ptree &data) {
  boost::property_tree::ptree response;

  std::cout << "OBJECT TYPE : " << properties.get<std::string>("object-type")
            << std::endl;
  if ("user" == objectType) {
    // dispatch crud user orders
    response = routeObjectType<user>(properties.get<std::string>("order-type"),
                                     properties, data);
  } else if ("group" == objectType) {
    // dispatch crud group orders
    response = routeObjectType<group>(properties.get<std::string>("order-type"),
                                      properties, data);
  } else if ("dictionary" == objectType) {
    // dispatch crud dictionary orders
    response = routeObjectType<dictionary>(
        properties.get<std::string>("order-type"), properties, data);
  } else if ("word" == objectType) {
    // dispatch crud word orders
    response = routeObjectType<word>(properties.get<std::string>("order-type"),
                                     properties, data);
  } else if ("party" == objectType) {
    // dispatch crud party orders
    response = routeObjectType<party>(properties.get<std::string>("order-type"),
                                      properties, data);
  } else if ("chat" == objectType) {
    // dispatch crud chat orders
    response = routeObjectType<chat>(properties.get<std::string>("order-type"),
                                     properties, data);
  } else if ("message" == objectType) {
    // dispatch crud message orders
    response = routeObjectType<message>(
        properties.get<std::string>("order-type"), properties, data);
  } else if ("team" == objectType) {
    // dispatch crud team orders
    response = routeObjectType<team>(properties.get<std::string>("order-type"),
                                     properties, data);
  } else {
    // unknown object type
    response.put("status-code", uint16_t(status_code::NOT_FOUND));
  }
  return response;
}
