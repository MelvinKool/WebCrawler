#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
#include <unordered_set>

namespace webcrawler
{
    class Crawler
    {
    private:
        int numThreads;
        std::queue<std::string> urlPool;
        std::unordered_set<std::string> foundURLs;
        void crawl(std::string& url);
    public:
        Crawler(int numberThreads);
        void start(std::string& startURL);
    };
}
#endif
