#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
namespace webcrawler
{
    class Crawler
    {
    private:
        int numThreads;
        std::queue<std::string> urlPool;
        void crawl(std::string& url);
    public:
        Crawler(int numberThreads);
        void start(std::string& startURL);
    };
}
#endif
