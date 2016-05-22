#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
#include "tinyxml2.h"

namespace webcrawler
{
    class Crawler
    {
    private:
        int numThreads;
        std::queue<std::string> urlPool;
        std::unordered_set<std::string> foundURLs;
        std::vector<std::string> extractLinks(tinyxml2::XMLElement* element,std::vector<std::string>& foundLinks,std::string& baseURL);
        void crawl(std::string& url);
    public:
        Crawler(int numberThreads);
        void start(std::string& startURL);
    };
}
#endif
