#include <string>
#include <iostream>
#include <stdexcept>
#include <regex>

#include "url.h"

// URL::URL(std::string url){
//     this->url = url;
// }

// bool URL::isAbsolute(){
//
// }
//
// bool URL::isRelative(){
//
// }

void URL::setURL(const std::string &url) {
    this->url = url;
}

std::string URL::toString() const {
    return url;
}

std::string URL::getFirstPiece(const std::string &url) {
    std::size_t found = url.find("//");
    if (found == std::string::npos) {
        found = url.find("/");
    }
    else {
        found = url.find("/", found + 2);
    }
    if (found == std::string::npos) {
        return url;
    }
    std::string firstPiece = url.substr(0, found);
    return firstPiece;
}

bool URL::isValidAbsolute() const {
    const std::string urlRegexStr = "(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))"
                                    "([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?";
    const std::regex urlRegex(urlRegexStr.c_str());
    return std::regex_match(url, urlRegex);
}

/*
    example.com/index/index.php -> example.com/index/
*/
std::string URL::toBaseURL(const std::string &url) {
    std::string baseURL = "";
    // if the url is http://example.com, ignore the / in the :// part
    std::size_t toIgnore = url.find("://");
    std::size_t found = url.find_last_of("/");
    if (found == std::string::npos || found < toIgnore + 3) {
        baseURL = url + "/";
    }
    else {
        baseURL = url.substr(0, found + 1);
    }
    return baseURL;
}

void URL::toAbsolute(const std::string &relativeToUrl) {
    if (isValidAbsolute()) {
        return;
    }
    //convert to base url: example.com/example/ instead of example.com/example/index.php
    std::string baseURL = URL::toBaseURL(relativeToUrl);
    std::string absoluteUrl = "";
    std::size_t found = url.find("/");
    if (found != std::string::npos) {
        //url begins with /
        if (found == 0) {
            // domain name<relative path>
            //get the domain name
            absoluteUrl = URL::getFirstPiece(relativeToUrl) + this->url;
        }
        //there is a / , so subfolder
        else {
            //find last / of base url and append the url to this piece
            absoluteUrl = baseURL + this->url;
        }
    }
    //url is something like second.php
    else {
        //append the url to base url
        absoluteUrl = baseURL + this->url;
    }
    this->url = absoluteUrl;
}
