#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
#include <condition_variable>
#include <mutex>
// #include "tinyxml2.h"
#include "gumbo.h"
#include "threadpool.h"

namespace webcrawler
{
    class Crawler
    {
    private:
        static void extractLinks(GumboNode* node,std::vector<std::string>& foundLinks,std::string& relativeToUrl);
        void crawl(std::string& url);
    public:
        Crawler(int numberThreads);
        ~Crawler();
        void start(std::string& startURL);
        void stop();
    private:
        bool stopped;
        int numThreads;
        ThreadPool* pool;
        std::queue<std::string> urlPool;
        std::unordered_set<std::string> foundURLs;
        std::condition_variable* urlsInPool;
        std::mutex url_mut;
        std::mutex found_mut;
    };
}
#endif
