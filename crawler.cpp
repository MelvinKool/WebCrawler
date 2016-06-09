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

// #include "tinyxml2.h"
#include "gumbo.h"
#include "webcurl.h"
#include "crawler.h"
#include "url.h"

namespace webcrawler
{
    Crawler::Crawler(int numThreads){
        this->numThreads = numThreads;
    }

    void Crawler::start(std::string& startURL){
        crawl(startURL);
        while(!urlPool.empty()){
            std::string nextURL = urlPool.front();
            urlPool.pop();
            crawl(nextURL);
        }
    }

    void Crawler::extractLinks(GumboNode* node,std::vector<std::string>& foundLinks,std::string& relativeToUrl) {
        if (node->type != GUMBO_NODE_ELEMENT)
            return;
        GumboAttribute* href;
        if (node->v.element.tag == GUMBO_TAG_A &&
          (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
              std::cout << href->value << std::endl;
              std::cout << "relativeToUrl URL: " << relativeToUrl << std::endl;
              // URL url(std::string(foundLink));
              URL url;
              url.setURL(std::string(href->value));
              if(!url.isValidAbsolute())
                  url.toAbsolute(relativeToUrl);
              std::cout << "FINAL LINK: " << url.toString() << std::endl;
              foundLinks.push_back(url.toString());
        }
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i)
            extractLinks(static_cast<GumboNode*>(children->data[i]),foundLinks,relativeToUrl);
    }

    void Crawler::crawl(std::string& url){
    	std::string pageContent;
            try{
                pageContent = WebCurl::getPage(url);
            }
            catch(std::runtime_error err){
                std::cout << "AN ERROR OCCURED: " << err.what() << url << std::endl;
                return;//change this is the future
            }
        	MYSQL *conn;
        	conn = mysql_init(NULL);
        	if(!mysql_real_connect(conn,"localhost","root", "Timjar00", "LINKDB", 0, NULL, 0)){
        		printf("Dit gaat niet goed");
        	}
            std::vector<std::string> links;
            GumboOutput* output = gumbo_parse(pageContent.c_str());
            extractLinks(output->root,links,url);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
            for(std::string link : links){
                std::string query;
    	    if(foundURLs.find(link) == foundURLs.end()){
                //add the url to foundurls, so the crawler won't download the page again
                foundURLs.insert(link);
                urlPool.push(link);
        		query = "INSERT INTO links VALUES('"+ link +"')";
        		mysql_query(conn,query.c_str());
            }
        }
	    mysql_close(conn);
    }
}
