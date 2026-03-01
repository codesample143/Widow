#include <queue>
#include <string>
#include <curl/curl.h>

using namespace std;

class Crawler{
    private:
        CURL *curl;
        CURLcode res;
        queue<string> crawler_queue; //replace with generic pop lock safe like redis
        string starting_link;
    public:
        Crawler(string link);
        void crawl();
        void fetch(const std::string& url);
};