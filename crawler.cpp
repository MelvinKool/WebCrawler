#include <regex>
#include <string>
#include <iostream>
#include <stdexcept>

#include "webcurl.h"
#include "crawler.h"

namespace webcrawler
{
    Crawler::Crawler(std::string startURL, int numThreads){
        this->numThreads = numThreads;
        urlPool.push(startURL);
    }

    void Crawler::start(){
        std::string firstUrl = urlPool.front();
        urlPool.pop();
        crawl(firstUrl);
    }

    void Crawler::crawl(std::string url){
        std::string pageContent;
        try{
            pageContent = WebCurl::getPage(url);
        }
        catch(std::runtime_error err){
            std::cout << "AN ERROR OCCURED: " << err.what() << std::endl;
            return;//change this is the future
        }
        // std::cout << pageContent << std::endl;
        const std::string urlRegexStr = "(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))"
                                        "([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?";
        const std::regex urlRegex(urlRegexStr.c_str());
        std::smatch sm;
        std::string::const_iterator searchStart( pageContent.cbegin() );
        while (std::regex_search(searchStart, pageContent.cend() ,sm, urlRegex))
        {
            for (auto it : sm)
            {
                std::cout << it << std::endl; // *i only yields the captured part
            }
            searchStart += sm.position() + sm.length();
        }
    }
}
