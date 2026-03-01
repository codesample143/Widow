#include <queue>
#include <iostream>
#include <thread>

#include <curl/curl.h>
#include "crawler.hpp"

using namespace std;

/**
 * Struct->
 *   Parser
 *   Request
 *   Recursive Search
 */

int main(){
    //this is very similar to a file handle
    queue<string> crawler_queue;
    
    curl_global_init(CURL_GLOBAL_ALL);

    Crawler c("https://en.wikipedia.org/wiki/Shin_Jin-seo");
    c.crawl();
}