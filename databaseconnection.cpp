#include <iostream>
#include <string>
#include <vector>
#include <mysql_driver.h>
//#include <mysql_connection.h>
//#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
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

    // DatabaseConnection::~DatabaseConnection(){
    // }

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

    void DatabaseConnection::executeStatement(const char* statement){
        try{
            std::unique_ptr<sql::Statement> stmt(con->createStatement());
        	stmt->execute(statement);
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::createDB(){
        try{
        	executeStatement("CREATE DATABASE IF NOT EXISTS webcrawler;");
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::dropDB(){
        try{
        	executeStatement("DROP DATABASE IF EXISTS webcrawler;");
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::createTables(){
        try{
            executeStatement("CREATE TABLE IF NOT EXISTS links ("
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
            executeStatement("DROP TABLE IF EXISTS links;");
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    std::vector<std::string> DatabaseConnection::getLinksToCrawl(int amount, int daysAgo){
        std::string prep_stmt_str = "SELECT url FROM links WHERE crawled_on IS NULL OR crawled_on < DATE_SUB(NOW(), INTERVAL ? DAY) LIMIT ?;";
        std::vector<std::string> links;
        try{
            std::unique_ptr<sql::PreparedStatement> prep_stmt(con->prepareStatement(prep_stmt_str));
            prep_stmt->setInt(1,daysAgo);
            prep_stmt->setInt(2,amount);
            std::unique_ptr<sql::ResultSet> res(prep_stmt->executeQuery());
            while(res->next()){
                links.push_back(res->getString(1));
            }
        }
        catch(sql::SQLException &e){
            throw e;
        }
        return links;
    }
}
