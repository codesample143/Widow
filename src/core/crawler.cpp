#include <iostream>

#include "parser/parserhelper.hpp"
#include "crawler.hpp"

using namespace std;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

Crawler::Crawler(string link) : starting_link(link) {
    crawler_queue.push(starting_link);
}

void Crawler::crawl(){
    while (!crawler_queue.empty()) {
        std::string link = crawler_queue.front();
        crawler_queue.pop();
        fetch(link);
    }

    
}

void Crawler::fetch(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to init curl");
    }

    string buffer = "";

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Noir-Agent(1.0)");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    CURLcode res = curl_easy_perform(curl);

    //add error check
    curl_easy_cleanup(curl);
    
}