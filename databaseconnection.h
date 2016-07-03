#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include "databaseconnection.h"
namespace webcrawler{
        class DatabaseConnection{
        public:
            DatabaseConnection(std::string& host, std::string& dbSchema, std::string& user, std::string& password);
            void connect(std::string& host, std::string& dbSchema ,std::string& user,std::string& password);
            void createTables();
            void dropTables();
            //void insertLink(std::string& link);
        private:
            sql::Driver* driver;
            std::unique_ptr<sql::Connection> con;
        };
}
#endif
