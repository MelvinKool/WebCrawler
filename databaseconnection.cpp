#include <iostream>
#include <string>
#include <mysql_driver.h>
//#include <mysql_connection.h>
//#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include "databaseconnection.h"

namespace webcrawler{
    DatabaseConnection::DatabaseConnection(std::string& host, std::string& dbSchema, std::string& user, std::string& password){
        try{
            connect(host,dbSchema,user,password);
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::connect(std::string& host,std::string& dbSchema,std::string& user,std::string& password){
        try{
            driver = get_driver_instance();
            con = std::unique_ptr<sql::Connection>(driver->connect(host,user,password));
            con->setSchema(dbSchema);
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::createTables(){
        try{
            std::unique_ptr<sql::Statement> stmt(con->createStatement());
            stmt->execute("CREATE TABLE IF NOT EXISTS links ("
                           "id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
                           "url VARCHAR(500),"
                           "content TEXT,"
                           "crawled_on DATETIME"
                           ");");
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }


    void DatabaseConnection::dropTables(){
        try{
            std::unique_ptr<sql::Statement> stmt(con->createStatement());
            stmt->execute("DROP TABLE IF EXISTS links;");
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }
}
