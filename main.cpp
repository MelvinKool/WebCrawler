#include <iostream>
#include <string>
#include <cstdio>
#include <signal.h>
#include <assert.h>
#include <algorithm>
#include <curl/curl.h>
#include "crawler.h"
#include "database.h"

#include <mysql/mysql.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace webcrawler;

void showOptions(){
    std::cout << "WebCrawler v1.0" << std::endl << std::endl;
    std::cout << "--help : show help" << std::endl;
    std::cout << "--create : This must be initialized before use. It will create a database." << std::endl;
    std::cout << "-t <number> : Set amount of threads (default 4)" << std::endl;
    std::cout << "-a <link> : Set the link to begin downloading from" << std::endl;
    std::cout << "--delete : This can be used to fully delete your database with table." << std::endl;
    std::cout << "showall : this will fetch all the records from the database." << std::endl;
    
    //std::cout << "
    // std::cout << "-s <search term> : Search for links inside the database" << std::endl;
}

// void search(std::string find){
//     MYSQL *conn;
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     conn = mysql_init(NULL);
//     std::string query;
//     if(!mysql_real_connect(conn,"localhost","root", "Timjar00", "LINKDB", 0, NULL, 0)){
// 	printf("No Database connection.");
//     }
//     else{
// 	std::cout << "Enter a string to search for something specific(or leave blank to view full database): ";
// 	std::getline(std::cin,find);
// 	query = "SELECT * FROM links WHERE link LIKE '%"+ find +"%'";
// 	mysql_query(conn, query.c_str());
// 	res = mysql_use_result(conn);
// 	printf("Your search returned these results: \n");
// 	while ((row = mysql_fetch_row(res)) != NULL){
// 	    printf("%s \t\n",row[0]);
// 	}
// 	mysql_free_result(res);
// 	mysql_close(conn);
//     }
// }



bool is_number(const std::string& s){
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

//#define _AGENT      "WebCrawlerBot"

//TODO
void exit_handler(int s){
       printf("Caught signal %d\n",s);
       exit(1);
}


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
	    std::cout << "record #: " << count << "\n";
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

int main(int argc, char* argv[] )
{
    //ctrl c handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    /////////////////

    int numThreads = 4;
    std::string url;
    if(argc < 2 || (argc > 1 && std::string(argv[1]) == "--help")){
        showOptions();
        return 0;
    }
    else if(std::string(argv[1]) == "--create"){
	createDB();
	createTable();
        /*std::string host = "localhost";
        std::string user = "root";
        std::string password = "Timjar00";
        std::string dbname = "LINKDB";
        Database db(host,dbname,user,password);*/
	return 0;
    }
    else if(std::string(argv[1]) == "--delete"){
	deleteTable();
	dropDB();
	return 0;
    }
    else if(std::string(argv[1]) == "--showall"){
	fetchTableContent();
	std::cout << "============" << "\n";
	return 0;
    }
    // else if(std::string(argv[1]) == "-s"){
	// std::string z;
	// assert(z.c_str()); //aanpassen
	// search(argv[1]);
    // }
    else if(std::string(argv[1]) == "-t"){
        assert(argc > 2);
        std::string s(argv[2]);
        assert(is_number(s));
        numThreads = atoi(argv[2]);
    }
    else if(std::string(argv[1]) == "-a"){
        assert(argc > 2);
        //validate link?
        url = std::string(argv[2]);
    }
    
    Crawler crawler(numThreads);
    crawler.start(url);
    curl_global_cleanup(); //not thread safe, so only use in main
    return EXIT_SUCCESS;
}
