#include <curl/curl.h> //sudo aptitude install libcurl-dev && sudo apt-get install libcurl4-gnutls-dev
#include <iostream>
#include <string>
#include <stdexcept>
#include <assert.h>
#include <algorithm>
#include <regex>

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    ((std::string*)up)->append((char*)buf, size * nmemb);
    return size*nmemb; //tell curl how many bytes we handled
}

std::string getPage(std::string url){
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    code = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (code != CURLE_OK)
            throw std::runtime_error("Failed to retrieve web page");
    return readBuffer;
}

void showOptions(){
    std::cout << "WebCrawler v1.0" << std::endl << std::endl;
    std::cout << "--help : show help" << std::endl;
    std::cout << "-t <number> : Set amount of threads (default 4)" << std::endl;
    std::cout << "-a <link> : Set the link to begin downloading from" << std::endl;
}

bool is_number(const std::string& s){
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

int main(int argc, char* argv[] )
{
    int numThreads = 4;
    std::string url;
    if(argc < 2 || (argc > 1 && std::string(argv[1]) == "--help")){
        showOptions();
        return 0;
    }
    else if(std::string(argv[1]) == "-t"){
        assert(argc > 2);
        std::string s(argv[2]);
        assert(is_number(s));
        numThreads = atoi(argv[2]);
    }
    else if(std::string(argv[1]) == "-a"){
        assert(argc > 2);
        //validate link?
        url = std::string(argv[2]);
    }
    std::string pageContent = getPage(url);
    // std::cout << pageContent << std::endl;
    const std::string urlRegexStr = "(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))"
                                    "([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?";
    std::cout << urlRegexStr << std::endl;
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
    //curl_global_cleanup(); niet thread safe, anders wel gebruiken
    return EXIT_SUCCESS;
}
