// file      : hello/database.hxx
// @see https://github.com/igor-sadchenko/odb-examples/blob/master/hello/database.hxx
// copyright : not copyrighted - public domain

//
// Create concrete database instance based on the DATABASE_* macros.
//

#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <string>
#include <memory>   // std::unique_ptr
#include <iostream>

#include <odb/database.hxx>

#if defined(DATABASE_MYSQL)
#  include <odb/mysql/database.hxx>
#elif defined(DATABASE_SQLITE)
#  include <odb/connection.hxx>
#  include <odb/transaction.hxx>
#  include <odb/schema-catalog.hxx>
#  include <odb/sqlite/database.hxx>
#elif defined(DATABASE_PGSQL)
#  include <odb/pgsql/database.hxx>
#elif defined(DATABASE_ORACLE)
#  include <odb/oracle/database.hxx>
#elif defined(DATABASE_MSSQL)
#  include <odb/mssql/database.hxx>
#else
#  error unknown database; did you forget to define the DATABASE_* macros?
#endif

inline std::unique_ptr<odb::database>
create_database (int& argc, char* argv[])
{
  using namespace odb::core;

  if (argc > 1 && argv[1] == std::string ("--help"))
  {
    std::cout << "Usage: " << argv[0] << " [options]" << std::endl
         << "Options:" << std::endl;

#if defined(DATABASE_MYSQL)
    odb::mysql::database::print_usage (std::cout);
#elif defined(DATABASE_SQLITE)
    odb::sqlite::database::print_usage (std::cout);
#elif defined(DATABASE_PGSQL)
    odb::pgsql::database::print_usage (std::cout);
#elif defined(DATABASE_ORACLE)
    odb::oracle::database::print_usage (std::cout);
#elif defined(DATABASE_MSSQL)
    odb::mssql::database::print_usage (std::cout);
#endif
  } else {

#if defined(DATABASE_MYSQL)
  std::unique_ptr<database> db (new odb::mysql::database (argc, argv));
#elif defined(DATABASE_SQLITE)
  std::unique_ptr<database> db (
     new odb::sqlite::database (
       argc, argv, false, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
  // Create the database schema. Due to bugs in SQLite foreign key
  // support for DDL statements, we need to temporarily disable
  // foreign keys.
  //
    const connection_ptr c (db->connection ());

    c->execute ("PRAGMA foreign_keys=OFF");

    transaction t (c->begin ());
    schema_catalog::create_schema (*db);
    t.commit ();

    c->execute ("PRAGMA foreign_keys=ON");
#elif defined(DATABASE_PGSQL)
  std::unique_ptr<database> db (new odb::pgsql::database (argc, argv));
#elif defined(DATABASE_ORACLE)
  std::unique_ptr<database> db (new odb::oracle::database (argc, argv));
#elif defined(DATABASE_MSSQL)
  std::unique_ptr<database> db (new odb::mssql::database (argc, argv));
#endif
  }
  return db;
}

#endif // DATABASE_HXX
