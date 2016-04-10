#include <curl/curl.h> //sudo aptitude install libcurl-dev && sudo apt-get install libcurl4-gnutls-dev
#include <iostream>
#include <string>

std::string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

int main(int argc, char* argv[] )
{
        CURLcode code;
        CURL *curl;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl == NULL)
        {
                fprintf(stderr, "Failed to create CURL connection\n");
                exit(EXIT_FAILURE);
        }
        curl_easy_setopt(curl, CURLOPT_URL, "");
        if (code != CURLE_OK)
        {
                exit(EXIT_FAILURE);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
        code = curl_easy_perform(curl);
        std::cout << data << std::endl;
        curl_easy_cleanup(curl);
        if (code != CURLE_OK)
        {
                exit(EXIT_FAILURE);
        }
        curl_global_cleanup();
        return EXIT_SUCCESS;
}
