#include <iostream>
#include <string>
#include <cstdio>
#include <signal.h>
#include <assert.h>
#include <algorithm>
#include <curl/curl.h>
#include <cppconn/exception.h>
#include <string>
#include "crawler.h"
#include "config.h"
#include "databaseconnection.h"
#include <termios.h>
#include <unistd.h>
using namespace webcrawler;

std::string db_host;
std::string db_name;
std::string db_user;
std::string db_password;
void showOptions(){
    std::cout << "WebCrawler v1.0" << std::endl << std::endl;
    std::cout << "--help : show help" << std::endl;
    std::cout << "-t <number> : Set amount of threads (default 4)" << std::endl;
    std::cout << "-a <link> : Set the link to begin downloading from" << std::endl;
    std::cout << "--create : Create a fresh database" << std::endl;
    // std::cout << "-s <search term> : Search for links inside the database" << std::endl;
}

bool is_number(const std::string& s){
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool yesNo(){

    std::string answer;
    std::getline(std::cin,answer);
    return answer == "y";
}

void askForDBCredentials(){
    std::string host,user,name;
    std::cout << "Database host: ";
    std::getline(std::cin,host);
    db_host = host;
    std::cout << "Database: ";
    std::getline(std::cin,name);
    db_name = name;
    std::cout  << "Username: ";
    std::getline(std::cin,user);
    db_user = user;
    std::cout  << "Password: ";
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::string password;
    std::getline(std::cin,password);
    db_password = password;
    std::cout << std::endl;
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

bool getDBCredentials(){
    bool authenticationSuccessful = false;
    do{
        askForDBCredentials();
        try{
            DatabaseConnection db(db_host,db_user,db_password);
            authenticationSuccessful = true;
        }
        catch(sql::SQLException &e){
            std::cout << "Authentication failed. Try again? y/n: ";
            if(!yesNo()) return false;
            std::cout << "Please re-enter your credentials:" << std::endl;
        }
    }
    while(!authenticationSuccessful);
    return true;
}

// void mysql_cleanup(){
//     sql::Driver::threadEnd();
// }

void cleanup(){
    curl_global_cleanup(); //not thread safe, so only use in main
    // mysql_cleanup();
}

//TODO
void exit_handler(int s){
       printf("Caught signal %d\n",s);
       cleanup();
       exit(1);
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

    int numThreads = 8;
    std::string url;
    if(argc < 2 || (argc > 1 && std::string(argv[1]) == "--help")){
        showOptions();
        return 0;
    }
    // sql::Driver::threadInit();
    if(!getDBCredentials()) return EXIT_FAILURE;
    if(std::string(argv[1]) == "--create"){
        std::cout << "Connecting to db..." << std::endl;
        DatabaseConnection db(db_host,db_user,db_password);
        try{
            db.setSchema(db_name);
            std::cout << "It seems like the database already exists. Drop current tables? y/n: ";
            if(yesNo())
                db.dropTables();
            else
                return EXIT_FAILURE;
        }
        catch(sql::SQLException &e){
            db.createDB(db_name);
            db.setSchema(db_name);
        }
        std::cout << "Creating tables..." << std::endl;
        db.createTables();
        std::cout << "Creating tables done." << std::endl;
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
    cleanup();
    return EXIT_SUCCESS;
}
