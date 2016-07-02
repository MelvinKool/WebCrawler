#include <iostream>
#include <string>
#include <mysql_driver.h>
//#include <mysql_connection.h>
//#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include "database.h"

namespace webcrawler{
    Database::Database(std::string& host, std::string& dbSchema, std::string& user, std::string& password){
        dbConn(host,dbSchema,user,password);
    }

    Database::~Database(){
        //close Connection
        //TODO
        std::cout << "Database Deconstructor working..." << std::endl;
        // delete driver;
        delete con;
    }

    void Database::dbConn(std::string& host,std::string& dbSchema,std::string& user,std::string& password){
        try{
            driver = get_driver_instance();
            con = driver->connect(host,user,password);
            con->setSchema(dbSchema);
            std::cout << "jup" << std::endl;
        }
        catch(sql::SQLException &e){
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

/*try{
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *prep_stmt;
    driver = get_driver_instance();
    con = driver->connect("localhost","root", "Timjar00");
    con->setSchema("LINKDB");
    prep_stmt = con->prepareStatement("INSERT INTO links VALUES ('?');");
    prep_stmt->setString(1,link);
    prep_stmt->execute();
    delete prep_stmt;
    delete con;
}
catch(sql::SQLException &e){
    std::cout << "Error: " << e.what() << std::endl;
}*/
