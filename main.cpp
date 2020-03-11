#include <iostream>
#include <string>
#include <cstdio>
#include <csignal>
#include <cassert>
#include <algorithm>
#include <curl/curl.h>
#include <mysql/mysql.h>
#include "cxxopts.hpp"
#include "crawler.h"

void search(std::string find) {
    MYSQL *conn = mysql_init(nullptr);
    std::string query;
    if (!mysql_real_connect(conn, "localhost", "root", "Timjar00", "LINKDB", 0, nullptr, 0)) {
        puts("No Database connection.");
    }
    else {
        std::cout << ("Enter a string to search for something specific(or leave blank to view full database): ");
        std::getline(std::cin, find);
        query = "SELECT * FROM links WHERE link LIKE '%" + find + "%'";
        mysql_query(conn, query.c_str());
        MYSQL_RES *res = mysql_use_result(conn);
        puts("Your search returned these results: ");
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            printf("%s\n", row[0]);
        }
        mysql_free_result(res);
        mysql_close(conn);
    }
}

//#define _AGENT      "WebCrawlerBot"

//TODO
void exit_handler(int s) {
    printf("Caught signal %d\n", s);
    exit(1);
}

int main(int argc, char *argv[]) {
    //ctrl c handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exit_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);
    /////////////////

    int numThreads = 4;
    std::string url;
    bool needHelp = false;
    bool printVersion = false;
    std::string searchParam;
    std::string link;

    cxxopts::Options options(argv[0]);
    options.add_options()
        ("h, help", "Get help message", cxxopts::value<bool>(needHelp))
        ("v, version", "Get version", cxxopts::value<bool>(printVersion))
        ("t, threads", "Set amount of threads (default 4)", cxxopts::value<int>(numThreads))
        ("a, link", "Set the link to begin downloading from", cxxopts::value<std::string>(link))
        ("s, search", "Search for links inside the database", cxxopts::value<std::string>(searchParam));
    options.parse(argc, argv);

    if (needHelp)
    {
        puts(options.help().c_str());
        return 0;
    }
    if (printVersion)
    {
        puts("WebCrawler 1.0");
        return 0;
    }
    if (!searchParam.empty()) {
        search(searchParam);
    }
    else if (!link.empty()) {
        url = link;
    }

    webcrawler::Crawler crawler(numThreads);
    crawler.start(url);
    curl_global_cleanup(); //not thread safe, so only use in main
    return EXIT_SUCCESS;
}
