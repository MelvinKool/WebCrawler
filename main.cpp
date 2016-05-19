#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>
#include <curl/curl.h>

#include "crawler.h"

using namespace webcrawler;

void showOptions(){
    std::cout << "WebCrawler v1.0" << std::endl << std::endl;
    std::cout << "--help : show help" << std::endl;
    std::cout << "-t <number> : Set amount of threads (default 4)" << std::endl;
    std::cout << "-a <link> : Set the link to begin downloading from" << std::endl;
}

bool is_number(const std::string& s){
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

//#define _AGENT      "WebCrawlerBot"

int main(int argc, char* argv[] )
{
    int numThreads = 4;
    std::string url;
    if(argc < 2 || (argc > 1 && std::string(argv[1]) == "--help")){
        showOptions();
        return 0;
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
