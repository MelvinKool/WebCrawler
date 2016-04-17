#ifndef WCURL_H
#define WCURL_H

#include <string>

class WebCurl {
private:
    static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
public:
    static std::string getPage(std::string& url);
};
#endif
