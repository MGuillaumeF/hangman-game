#ifndef __DATA_ACCESS_HPP__
#define __DATA_ACCESS_HPP__

#include <memory>

// generated configuration
#include "config.hpp"

#include <odb/transaction.hxx>

// create database access
#include "../connector/database.hxx"

class DataAccess {
  static DataAccess *s_instance;
  odb::core::database *m_db;
  DataAccess();
  ~DataAccess() = default;

public:
  static odb::core::database *getDatabaseAccess();
  odb::core::database *getDataBase();
};

#endif