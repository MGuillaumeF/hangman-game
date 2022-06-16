// file      : hello/database.hxx
// @see      :
// https://github.com/igor-sadchenko/odb-examples/blob/master/hello/database.hxx
// copyright : not copyrighted - public domain

//
// Create concrete database instance based on the DATABASE_* macros.
//
#ifndef __DATABASE_HXX__
#define __DATABASE_HXX__

#include <iostream>
#include <memory> // std::unique_ptr
#include <stdexcept>
#include <string>

#include <odb/database.hxx>

// generated configuration
#include "config.hpp"

#if defined(DATABASE_MYSQL)
#include <odb/mysql/database.hxx>
#elif defined(DATABASE_SQLITE)
#include <odb/connection.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/transaction.hxx>

#elif defined(DATABASE_PGSQL)
#include <odb/pgsql/database.hxx>
#elif defined(DATABASE_ORACLE)
#include <odb/oracle/database.hxx>
#elif defined(DATABASE_MSSQL)
#include <odb/mssql/database.hxx>
#else
#error unknown database; did you forget to define the DATABASE_* macros?
#endif

inline odb::database *create_database(int &argc, char *argv[]) {
  odb::core::database *db = nullptr;
  if (argc > 1 && argv[1] == std::string("--help")) {
    std::cout << "Usage: " << argv[0] << " [options]" << std::endl
              << "Options:" << std::endl;

#if defined(DATABASE_MYSQL)
    odb::mysql::database::print_usage(std::cout);
#elif defined(DATABASE_SQLITE)
    odb::sqlite::database::print_usage(std::cout);
#elif defined(DATABASE_PGSQL)
    odb::pgsql::database::print_usage(std::cout);
#elif defined(DATABASE_ORACLE)
    odb::oracle::database::print_usage(std::cout);
#elif defined(DATABASE_MSSQL)
    odb::mssql::database::print_usage(std::cout);
#endif
    // compilation failed with unknown database message
  } else {

#if defined(DATABASE_MYSQL)
    db = odb::core::database(
        new odb::mysql::database(argc, argv);
#elif defined(DATABASE_SQLITE)
    db = new odb::sqlite::database(argc, argv, false,
                                   SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    // Create the database schema. Due to bugs in SQLite foreign key
    // support for DDL statements, we need to temporarily disable
    // foreign keys.
    //
    const odb::core::connection_ptr c(db->connection());

    c->execute("PRAGMA foreign_keys=OFF");

    odb::core::transaction t(c->begin());
    odb::core::schema_catalog::create_schema(*db);
    t.commit();

    c->execute("PRAGMA foreign_keys=ON");
#elif defined(DATABASE_PGSQL)
    db = new odb::pgsql::database(argc, argv);
#elif defined(DATABASE_ORACLE)
    db = new odb::oracle::database(argc, argv);
#elif defined(DATABASE_MSSQL)
    db = new odb::mssql::database(argc, argv);
#endif
  }
  return db;
}

#endif // DATABASE_HXX
