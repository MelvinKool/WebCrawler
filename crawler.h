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
        void crawl(std::string url);
    public:
        Crawler(std::string startURL, int numberThreads);
        void start();
    };
}
#endif
