-- create user odb_test without password 
-- to add password add at the end of create : IDENTIFIED BY 'password'
GRANT ALL PRIVILEGES ON *.* TO 'odb_test'@'localhost';
CREATE DATABASE odb_test;