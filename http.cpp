#include "http.hpp"
#include <unistd.h>

HTTPAPI http;

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    *(std::stringstream*)userp << string((char*)buffer, size * nmemb);
}

size_t write_data_null(void *buffer, size_t size, size_t nmemb, void *userp) {}

HTTPHandle::HTTPHandle(const HTTPHandle& h) {
    code = h.code;
    handle = h.handle;
    headers = h.headers;
    data << h.data.str();
}

HTTPHandle::~HTTPHandle() {
    if (handle != NULL) curl_easy_cleanup(handle);
    if (headers != NULL) curl_slist_free_all(headers);
}

int HTTPHandle::getResponseCode() {
    return code;
}

void HTTPHandle::close() {
    if (handle != NULL) curl_easy_cleanup(handle);
    if (headers != NULL) curl_slist_free_all(headers);
    handle = NULL;
    headers = NULL;
}

string HTTPHandle::readLine() {
    string retval;
    std::getline(data, retval);
    return retval;
}

string HTTPHandle::readAll() {
    return data.str();
}

HTTPAPI::HTTPAPI() {curl_global_init(CURL_GLOBAL_SSL);}

void HTTPAPI::request(string url, string postData, std::map<string, string> headers) {
    pid_t PID = fork();
    if (PID != 0) return;
    CURL * handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data_null);
    if (postData != "") curl_easy_setopt(handle, CURLOPT_POSTFIELDS, postData.c_str());
    struct curl_slist *header_list = NULL;
    if (headers.size() > 0) {
        for (std::map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
            header_list = curl_slist_append(header_list, (it->first + ": " + it->second).c_str());
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, header_list);
    }
    curl_easy_perform(handle);
    if (headers.size() > 0) curl_slist_free_all(header_list);
}

HTTPHandle HTTPAPI::get(string url, std::map<string, string> headers) {
    HTTPHandle h;
    h.handle = curl_easy_init();
    curl_easy_setopt(h.handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(h.handle, CURLOPT_WRITEDATA, &h.data);
    if (headers.size() > 0) {
        for (std::map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
            h.headers = curl_slist_append(h.headers, (it->first + ": " + it->second).c_str());
        curl_easy_setopt(h.handle, CURLOPT_HTTPHEADER, h.headers);
    }
    if (curl_easy_perform(h.handle) != CURLE_OK) return h;
    h.code = curl_easy_getinfo(h.handle, CURLINFO_RESPONSE_CODE, &h.code);
    return h;
}

HTTPHandle HTTPAPI::post(string url, string postData, std::map<string, string> headers) {
    HTTPHandle h;
    h.handle = curl_easy_init();
    curl_easy_setopt(h.handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(h.handle, CURLOPT_WRITEDATA, &h.data);
    curl_easy_setopt(h.handle, CURLOPT_POSTFIELDS, postData.c_str());
    if (headers.size() > 0) {
        for (std::map<string, string>::iterator it = headers.begin(); it != headers.end(); it++)
            h.headers = curl_slist_append(h.headers, (it->first + ": " + it->second).c_str());
        curl_easy_setopt(h.handle, CURLOPT_HTTPHEADER, h.headers);
    }
    if (curl_easy_perform(h.handle) != CURLE_OK) return h;
    h.code = curl_easy_getinfo(h.handle, CURLINFO_RESPONSE_CODE, &h.code);
    return h;
}

bool HTTPAPI::checkURL(string url) {
    CURL * handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data_null);
    CURLcode c = curl_easy_perform(handle);
    curl_easy_cleanup(handle);
    return c == CURLE_OK;
}