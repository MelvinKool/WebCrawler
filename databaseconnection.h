#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <vector>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include "databaseconnection.h"
namespace webcrawler{
        class DatabaseConnection{
        public:
            DatabaseConnection(std::string& host, std::string& dbSchema, std::string& user, std::string& password);
            // ~DatabaseConnection();
            void connect(std::string& host, std::string& dbSchema ,std::string& user,std::string& password);
            void executeStatement(const char* statement);
            void createDB();
            void dropDB();
            void createTables();
            void dropTables();
            std::vector<std::string> getLinksToCrawl(int amount, int daysAgo);
            //void insertLink(std::string& link);
        private:
            sql::Driver* driver;
            std::unique_ptr<sql::Connection> con;
        };
}
#endif
