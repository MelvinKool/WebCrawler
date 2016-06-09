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

    // std::vector<std::string> Crawler::extractLinks(tinyxml2::XMLElement* element,std::vector<std::string>& foundLinks,std::string& baseURL){
    //     if(std::string(element->Value()) == "a"){
    //         const char* foundLink = element->Attribute("href");
    //         if(foundLink != nullptr){
    //             std::cout << "FOUND LINK: " << foundLink << std::endl;
    //             std::cout << "BASE  URL: " << baseURL << std::endl;
    //             // URL url(std::string(foundLink));
    //             URL url;
    //             url.setURL(std::string(foundLink));
    //             if(!url.isValidAbsolute())
    //                 url.toAbsolute(baseURL);
    //             //bool b = url.isValidAbsolute();
    //             std::cout << "FINAL LINK: " << foundLink << std::endl;
    //             foundLinks.push_back(url.toString());
    //         }
    //     }
    //     for(tinyxml2::XMLElement* e = element->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    //     {
    //         extractLinks(e,foundLinks,baseURL);
    //     }
    //     return foundLinks;
    // }

    void Crawler::crawl(std::string& url){
	
	std::string pageContent;
        try{
            pageContent = WebCurl::getPage(url);  
        }
        catch(std::runtime_error err){
            std::cout << "AN ERROR OCCURED: " << err.what() << url << std::endl;
            return;//change this is the future
        }
        // tinyxml2::XMLDocument doc;
        // doc.Parse(pageContent.c_str());
        // if(doc.ErrorID() != 0){
        //     std::cout << "BAD XML" << std::endl;
        //     std::cout << doc.ErrorID() << std::endl;
        //     return;
        // }
        // tinyxml2::XMLElement* html = doc.FirstChildElement("html");
        // if (html == nullptr) return;// XML_ERROR_PARSING_ELEMENT;
        // tinyxml2::XMLElement* body = html->FirstChildElement("body");
        // if (body == nullptr) return;// XML_ERROR_PARSING_ELEMENT;
	MYSQL *conn;
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,"localhost","root", "Timjar00", "LINKDB", 0, NULL, 0)){
		printf("Dit gaat niet goed");
	}
        std::vector<std::string> links;
        // extractLinks(body,links,url);
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
        // const std::string urlRegexStr = "(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))"
        //                                 "([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?";
        // const std::regex urlRegex(urlRegexStr.c_str());
        // std::smatch sm;
        // std::string::const_iterator searchStart( pageContent.cbegin() );
        // while (std::regex_search(searchStart, pageContent.cend() ,sm, urlRegex))
        // {
        //     std::cout << sm[0] << std::endl;
        //     if(foundURLs.find(sm[0]) == foundURLs.end()){
        //         foundURLs.insert(sm[0]);
        //         urlPool.push(sm[0]);
        //     }
        //     searchStart += sm.position() + sm.length();
        // }
        // // std::this_thread::sleep_for(std::chrono::seconds(1));
        // if(!urlPool.empty()){
        //     std::string nextURL = urlPool.front();
        //     urlPool.pop();
        //     crawl(nextURL);
        // }
    }
}

