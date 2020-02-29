#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <queue>
#include <unordered_set>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <memory>
// #include "tinyxml2.h"
#include "gumbo.h"

class ThreadPool;

namespace webcrawler
{
    class Crawler
    {
    private:
        static void extractLinks(GumboNode *node, std::vector<std::string> &foundLinks, const std::string &relativeToUrl);
        void crawl(const std::string &url);
    public:
        explicit Crawler(int numberThreads);
        ~Crawler();
        void start(const std::string &startURL);
        void stop();
    private:
        bool stopped = false;
        std::unique_ptr<ThreadPool> pool;
        std::queue<std::string> urlPool;
        std::unordered_set<std::string> foundURLs;
        std::condition_variable urlsInPool;
        std::mutex url_mut;
        std::mutex found_mut;
    };
} // namespace webcrawler

#endif // CRAWLER_H
