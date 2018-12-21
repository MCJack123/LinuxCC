#include <string>
#include <map>
#include <sstream>
#include <curl/curl.h>

using std::string;

class HTTPHandle {
    friend class HTTPAPI;
    std::stringstream data;
    long code = 0;
    CURL * handle;
    struct curl_slist *headers = NULL;
public:
    HTTPHandle() {}
    HTTPHandle(const HTTPHandle& h);
    ~HTTPHandle();
    int getResponseCode();
    void close();
    string readLine();
    string readAll();
};

class HTTPAPI {
public:
    HTTPAPI();
    void request(string url, string postData = "", std::map<string, string> headers = {});
    HTTPHandle get(string url, std::map<string, string> headers = {});
    HTTPHandle post(string url, string postData, std::map<string, string> headers = {});
    bool checkURL(string url);
};

extern HTTPAPI http;