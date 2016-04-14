#ifndef WCURL_H
#define WCURL_H

#include <string>

class WebCurl {
private:
    size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
public:
    std::string getPage(std::string url);
};
#endif
