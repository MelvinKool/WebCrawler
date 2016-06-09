#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
// #include "tinyxml2.h"
#include "gumbo.h"
#include "threadpool.h"

namespace webcrawler
{
    class Crawler
    {
    private:
        ThreadPool pool;
        int numThreads;
        std::queue<std::string> urlPool;
        std::unordered_set<std::string> foundURLs;
        static void extractLinks(GumboNode* node,std::vector<std::string>& foundLinks,std::string& relativeToUrl);
        void crawl(std::string& url);
    public:
        Crawler(int numberThreads);
        void start(std::string& startURL);
    };
}
#endif
