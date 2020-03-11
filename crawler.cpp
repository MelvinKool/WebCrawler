#include <regex>
#include <string>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <sstream>
#include <vector>
#include <mysql/mysql.h>
//#include <mysql/my_global.h>
#include <functional>
#include <future>
#include <condition_variable>
// #include "tinyxml2.h"
#include <gumbo.h>
#include <spdlog/spdlog.h>
#include "webcurl.h"
#include "crawler.h"
#include "url.h"
#include "threadpool.h"

namespace webcrawler
{
    Crawler::Crawler(size_t numThreads) : pool(new ThreadPool(numThreads, urlsInPool)) {}

    Crawler::~Crawler() {
        stopped = true;
        urlsInPool.notify_all();
    }

    void Crawler::start(const std::string &startURL) {
        pool->enqueue([&] {
            //task
            crawl(startURL);
        });
        while (!stopped) {
            std::unique_lock<std::mutex> poolLock(url_mut);
            //check if the links pool is empty, if it is, wait for condition_variable, until it's not
            do {
                urlsInPool.wait(poolLock);
            }
            while (urlPool.empty() && !stopped);
            if (stopped) {
                poolLock.unlock();
                return;
            }
            //get amount of urls equal to amount of free workers and amount of links
            size_t workersFree = pool->getAmountFreeWorkers();
            // spdlog::info("Workers free: {}", workersFree);
            for (size_t i = 0; i < workersFree && i < urlPool.size(); i++) {
                //get an url to crawl
                std::string nextURL = urlPool.front();
                urlPool.pop();
                //crawl the next url
                pool->enqueue([&] {
                    //task
                    crawl(nextURL);
                });
            }
        }
    }

    void Crawler::stop() {
        stopped = true;
    }

    void Crawler::extractLinks(GumboNode *node, std::vector<std::string> &foundLinks, const std::string &relativeToUrl) {
        if (node->type != GUMBO_NODE_ELEMENT)
            return;
        GumboAttribute *href;
        if (node->v.element.tag == GUMBO_TAG_A &&
            (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
            // URL url(std::string(foundLink));
            URL url;
            url.setURL(std::string(href->value));
            if (!url.isValidAbsolute())
                url.toAbsolute(relativeToUrl);
            foundLinks.push_back(url.toString());
        }
        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            extractLinks(static_cast<GumboNode *>(children->data[i]), foundLinks, relativeToUrl);
        }
    }

    void Crawler::crawl(const std::string &url) {
        spdlog::info("Crawling {}", url);
        std::string pageContent;
        try {
            pageContent = WebCurl::getPage(url);
        }
        catch (const std::runtime_error &err) {
            spdlog::info("AN ERROR OCCURRED: {} {}", err.what(), url);
            return; //change this is the future
        }
        // MYSQL *conn;
        // conn = mysql_init(nullptr);
        // if(!mysql_real_connect(conn,"localhost","root", "Timjar00", "LINKDB", 0, nullptr, 0)){
        // 	printf("Dit gaat niet goed");
        // }
        std::vector<std::string> links;
        GumboOutput *output = gumbo_parse(pageContent.c_str());
        extractLinks(output->root, links, url);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        for (const std::string &link : links) {
            // std::string query;
            std::lock_guard<std::mutex> foundLock(found_mut);
            if (foundURLs.find(link) == foundURLs.end()) {
                //add the url to foundurls, so the crawler won't download the page again
                std::lock_guard<std::mutex> poolLock(url_mut);
                foundURLs.insert(link);
                urlPool.push(link);
                // query = "INSERT INTO links VALUES('"+ link +"')";
                // mysql_query(conn,query.c_str());
            }
        }
        // mysql_close(conn);
    }
} // namespace webcrawler
