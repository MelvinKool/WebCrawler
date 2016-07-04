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
    DatabaseConnection::DatabaseConnection(std::string& host, std::string& user, std::string& password){
        try{
            connect(host,user,password);
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    // DatabaseConnection::~DatabaseConnection(){
    // }

    void DatabaseConnection::connect(std::string& host, std::string& user, std::string& password){
        try{
            driver = get_driver_instance();
            con = std::unique_ptr<sql::Connection>(driver->connect(host,user,password));
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

    void DatabaseConnection::createDB(std::string& dbName){
        try{
            std::string prep_stmt_str = "CREATE DATABASE IF NOT EXISTS ?;";
            std::unique_ptr<sql::PreparedStatement> prep_stmt(con->prepareStatement(prep_stmt_str));
            prep_stmt->setString(1,dbName);
            prep_stmt->execute();
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::dropDB(std::string& dbName){
        try{
            std::string prep_stmt_str = "DROP DATABASE IF EXISTS ?";
            std::unique_ptr<sql::PreparedStatement> prep_stmt(con->prepareStatement(prep_stmt_str));
            prep_stmt->setString(1,dbName);
            prep_stmt->execute();
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::setSchema(std::string& dbName){
        con->setSchema(dbName);
    }

    void DatabaseConnection::createTables(){
        try{
            executeStatement("CREATE TABLE IF NOT EXISTS links ("
                            "id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
                            "url VARCHAR(500) NOT NULL UNIQUE,"
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

    void DatabaseConnection::insertLink(std::string& link){
        std::string insert_str = "INSERT IGNORE INTO links SET url=?;";
        try{
            std::unique_ptr<sql::PreparedStatement> prep_stmt(con->prepareStatement(insert_str));
            prep_stmt->setString(1,link);
            prep_stmt->execute();
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }

    void DatabaseConnection::insertContent(std::string& link,std::string& content){
        std::string insertContentStr = "UPDATE links SET content=?,crawled_on=NOW() WHERE url=?;";
        try{
            std::unique_ptr<sql::PreparedStatement> prep_stmt(con->prepareStatement(insertContentStr));
            prep_stmt->setString(1,content);
            prep_stmt->setString(2,link);
            prep_stmt->execute();
        }
        catch(sql::SQLException &e){
            throw e;
        }
    }
}
