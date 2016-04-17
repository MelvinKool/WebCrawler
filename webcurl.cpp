#include <stdexcept>
#include <string>
#include <curl/curl.h> //sudo aptitude install libcurl-dev && sudo apt-get install libcurl4-gnutls-dev

#include "webcurl.h"

size_t WebCurl::writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    ((std::string*)up)->append((char*)buf, size * nmemb);
    return size*nmemb; //tell curl how many bytes we handled
}

std::string WebCurl::getPage(std::string& url){
    CURLcode code;
    CURL *curl;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl == NULL)
            throw std::runtime_error("Failed to create CURL connection");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    // if (code != CURLE_OK)
    //         throw std::runtime_error("There is something wrong with your URL");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WebCurl::writeCallback);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    code = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (code != CURLE_OK)
            throw std::runtime_error("Failed to retrieve web page");
    return readBuffer;
}
