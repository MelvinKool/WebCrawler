#include <iostream>
#include <string>
#include <cstdio>
#include <signal.h>
#include <assert.h>
#include <algorithm>
#include <curl/curl.h>
#include <mysql/mysql.h>
#include "crawler.h"

using namespace webcrawler;

void showOptions(){
    std::cout << "WebCrawler v1.0" << std::endl << std::endl;
    std::cout << "--help : show help" << std::endl;
    std::cout << "-t <number> : Set amount of threads (default 4)" << std::endl;
    std::cout << "-a <link> : Set the link to begin downloading from" << std::endl;
    std::cout << "-s <search term> : Search for links inside the database" << std::endl;
}

void search(std::string find){
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    conn = mysql_init(NULL);
    std::string query;
    if(!mysql_real_connect(conn,"localhost","root", "Timjar00", "LINKDB", 0, NULL, 0)){
	printf("No Database connection.");
    }
    else{
	std::cout << "Enter a string to search for something specific(or leave blank to view full database): ";
	std::getline(std::cin,find);
	query = "SELECT * FROM links WHERE link LIKE '%"+ find +"%'";
	mysql_query(conn, query.c_str());
	res = mysql_use_result(conn);
	printf("Your search returned these results: \n");
	while ((row = mysql_fetch_row(res)) != NULL){
	    printf("%s \t\n",row[0]);
	}
	mysql_free_result(res);
	mysql_close(conn);
    }
}



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
    else if(std::string(argv[1]) == "-s"){
	std::string z;
	assert(z.c_str()); //aanpassen
	search(argv[1]);
    }
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
