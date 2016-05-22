#ifndef URL_H
#define URL_H

#include <string>

class URL{
private:
    std::string url;
public:
    URL(std::string url);
    // bool normalize();
    // bool isAbsolute();
    // bool isRelative();
    bool isValid();
    std::string toString();
    void toAbsolute();
};
#endif
