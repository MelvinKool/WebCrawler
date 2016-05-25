#ifndef URL_H
#define URL_H

#include <string>

class URL{
private:
    std::string url;
public:
    // URL(std::string url);
    URL(){}
    // bool isAbsolute();
    // bool isRelative();
    void setURL(std::string url);
    static std::string getFirstPiece(std::string& url);
    static std::string toBaseURL(std::string& url);
    bool isValidAbsolute();
    std::string toString();
    void toAbsolute(std::string& relativeToUrl);
};
#endif
