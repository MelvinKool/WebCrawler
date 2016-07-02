#ifndef DATABASE_H
#define DATABASE_H
#include <mysql_connection.h>
#include <cppconn/driver.h>
namespace webcrawler{
        class Database{
        public:
            Database(std::string& host, std::string& dbSchema, std::string& user, std::string& password);
            ~Database();
            void dbConn(std::string& host, std::string& dbSchema ,std::string& user,std::string& password);
            //void insertLink(std::string& link);
        private:
            sql::Driver *driver;
            sql::Connection *con;
        };
}
#endif
