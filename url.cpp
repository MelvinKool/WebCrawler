#include <string>
#include <iostream>
#include <stdexcept>
#include <regex>

URL::URL(std::string url){
    this->url = url;
}

// bool URL::isAbsolute(){
//
// }
//
// bool URL::isRelative(){
//
// }

bool URL::isValidAbsolute(){
    const std::string urlRegexStr = "(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))"
                                    "([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?";
    const std::regex urlRegex(urlRegexStr.c_str());
    return std::regex_match(this->url,urlRegex);
}

// std::string URL::toString(){
//
// }

/*
    example.com/index/index.php -> example.com/index/
*/
static std::string URL::toBaseURL(std::string url){
    std::string baseURL = "";
    std::size_t found = this->url.find_last_of("/");
    if(found == std::string::npos){
        baseURL = this->url + "/";
    }
}

void URL::toAbsolute(std::string& relativeToUrl){
    if(isValid())
        return;
    //convert to base url: example.com/example/ instead of example.com/example/index.php
    std::string baseURL = URL::toBaseURL(relativeToUrl);
    std::string absoluteUrl = "";
    std::size_t found = this->url.find("/");
    if(found != std::string::npos){
        //begins with /
        if(found == 0){
            // domain name<relative path>
            //get the domain name
            // absoluteUrl = domain + this->url
        }
        //there is a / , so subfolder
        else{
            //find last / of base url and append de url to this piece
        }
    }
    //url is something like localhost or second.php
    else{
        //append the url to base url
        url = baseURL +
    }
}
