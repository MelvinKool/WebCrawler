#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


void createTable(){
    try{
        sql::Driver *driver;
        sql::Connection *conn;
        //sql::Statement *stmt;
        sql::PreparedStatement *prep_stmt;
        // sql::ResultSet *res;
        driver = get_driver_instance();
        std::cout << "Creating table inside your database..." << std::endl;
	sleep(2);
        conn = driver->connect("localhost","root", "Timjar00");
        conn->setSchema("LINKDB");
        prep_stmt = conn->prepareStatement("CREATE TABLE IF NOT EXISTS links ("
                                           "id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
                                           "url VARCHAR(500),"
                                           "content TEXT,"
                                           "crawled_on DATETIME"
                                           ");");
	
        prep_stmt->execute();
        delete prep_stmt;
        delete conn;
    }
    catch(sql::SQLException &e){
        std::cout << "Error: " << e.what() << std::endl;

    }
}

void deleteTable(){
    try{
        sql::Driver *driver;
        sql::Connection *conn;
        //sql::Statement *stmt;
        sql::PreparedStatement *prep_stmt;
        // sql::ResultSet *res;
        driver = get_driver_instance();
        std::cout << "Deleting table inside your database..." << std::endl;
	sleep(2);
        conn = driver->connect("localhost","root", "Timjar00");
        conn->setSchema("LINKDB");
        prep_stmt = conn->prepareStatement("DROP TABLE IF EXISTS links;");
	
        prep_stmt->execute();
        delete prep_stmt;
        delete conn;
    }
    catch(sql::SQLException &e){
        std::cout << "Error: " << e.what() << std::endl;

    }
}

void createDB(){
    try{
	sql::Driver *driver;
        sql::Connection *conn;
	sql::PreparedStatement *prep_stmt;
	driver = get_driver_instance();
	conn = driver->connect("localhost","root", "Timjar00");
	std::cout << "Creating Database..." << std::endl;
	sleep(2);
	prep_stmt = conn->prepareStatement("CREATE DATABASE IF NOT EXISTS LINKDB;");
	prep_stmt->execute();
	delete prep_stmt;
        delete conn;
	
	
    }
    catch(sql::SQLException &e){
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void dropDB(){
    try{
	sql::Driver *driver;
        sql::Connection *conn;
	sql::PreparedStatement *prep_stmt;
	driver = get_driver_instance();
	conn = driver->connect("localhost","root", "Timjar00");
	prep_stmt = conn->prepareStatement("DROP DATABASE IF EXISTS LINKDB;");
	prep_stmt->execute();
	delete prep_stmt;
        delete conn;
	std::cout << "deleting database..." << std::endl;
	sleep(2);
    }
    catch(sql::SQLException &e){
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void fetchTableContent(){
    try{
	sql::Driver *driver;
        sql::Connection *conn;
	sql::Statement *stmt;
	//sql::PreparedStatement *prep_stmt;
 	sql::ResultSet *res;
	driver = get_driver_instance();
	conn = driver->connect("localhost","root", "Timjar00");
	conn->setSchema("LINKDB");
	stmt = conn->createStatement();
	res = stmt->executeQuery("SELECT * FROM links;");
	std::cout << "fetching database records..." << std::endl;
	sleep(2);
	int count = 0;
	while(res->next()){
	    count=count + 1;
	    std::cout << "============" << "\n";
	    std::cout << "record # " << count << "\n";
	    std::cout << "============" << "\n";
	    std::cout << "id: " << res->getInt(1) << "\n";
	    std::cout << "url: " << res->getString(2) << "\n";
	    std::cout << "content: " << res->getString(3) << "\n";
	    std::cout << "crawled on: " << res->getString(4) << "\n";
	    //std::cout << "============" << "\n";
	}
	delete stmt;
        delete conn;
	delete res;
    }
    catch(sql::SQLException &e){
        std::cout << "Error: " << e.what() << std::endl;
    }
}

int main(){
    //createTable();
    createTable();
    return 0;
}









//prep_stmt = conn->prepareStatement("SELECT * FROM pikkie WHERE crawled_on IS NULL OR crawled_on < DATE_SUB(NOW(), INTERVAL 1 DAY) LIMIT 200;");
/*//url moet not null worden
CREATE DATABASE IF NOT EXISTS DBName;
DROP DATABASE IF EXISTS DBName;

//table maken
CREATE TABLE IF NOT EXISTS links (
id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
url VARCHAR(500),
content TEXT,
crawled_on DATETIME
);
// entries ophalen die korter dan 10 dagen in de db staan
SELECT * FROM links WHERE crawled_on IS NULL OR crawled_on < DATE_SUB(NOW(), INTERVAL 10 DAY) LIMIT 200;





INSERT INTO links (crawled_on,url) VALUES(STR_TO_DATE('06/29/2016 8:06:26','%c/%e/%Y %r'),'tietjs');
*/
