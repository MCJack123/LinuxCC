#include "io.hpp"
#include "fs.hpp"
#include <stdlib.h>
#include <string.h>

IOAPI io;

IOHandle::IOHandle(string path, string mode) {
    fp = fopen(path.c_str(), mode.c_str());
    this->mode = mode;
    this->path = path;
}

IOHandle::~IOHandle() {
    if (fp != NULL) fclose(fp);
}

void IOHandle::close() {
    if (fp != NULL) fclose(fp);
    fp = NULL;
}

void IOHandle::flush() {
    if (!(fp == NULL || mode.find('w') == string::npos)) fflush(fp);
}

IOHandleIterator IOHandle::lines() {
    char * buf;
    size_t read = 0;
    IOHandleIterator it;
    while (getline(&buf, &read, fp)) {
        it.list.push_back(string(buf, read));
        free(buf);
    }
    return it;
}

string IOHandle::read(string format = "*l") {
    if (fp == NULL || mode.find('r') == string::npos) return "";
    if (format == "*a" && path != "") {
        int len = fs.getSize(path) - ftell(fp);
        if (len < 1) return "";
        char * buf = (char*)malloc(len);
        fread(buf, len, 1, fp);
        string retval(buf);
        free(buf);
        return retval;
    } else {
        char * line = NULL;
        if (getline(&line, NULL, fp) == -1) return "";
        string retval(line);
        free(line);
        return retval;
    }
}

void IOHandle::write(string v) {
    if (fp == NULL || mode.find('w') == string::npos) return;
    fwrite(v.c_str(), v.size()+1, 1, fp);
}

void IOHandle::write(int n) {
    if (fp == NULL || mode.find('w') == string::npos) return;
    char str[33];
    sprintf(str, "%d\0", n);
    fwrite(str, strlen(str)+1, 1, fp);
}

void IOHandle::write(double n) {
    if (fp == NULL || mode.find('w') == string::npos) return;
    char str[33];
    sprintf(str, "%f\0", n);
    fwrite(str, strlen(str)+1, 1, fp);
}

string IOAPI::type(IOHandle obj) {
    if (obj.fp == NULL) return "closed file";
    else return "file";
}