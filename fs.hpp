#ifndef FSAPI_HPP
#define FSAPI_HPP
#include <string>
#include <vector>
#include <cstdio>

using std::string;

class FileHandle {
    FILE * fp;
    string mode;
    string path;
public:
    FileHandle(string path, string mode);
    ~FileHandle();
    void close();
    string readLine();
    string readAll();
    void write(string data);
    void writeLine(string data);
    void flush();
    char read();
    void write(char byte);
};

class FSAPI {
public:
    std::vector<string> list(string path);
    bool exists(string path);
    bool isDir(string path);
    bool isReadOnly(string path);
    string getName(string path);
    string getDrive(string path);
    int getSize(string path);
    int getFreeSpace(string path);
    bool makeDir(string path);
    bool move(string fromPath, string toPath);
    bool copy(string fromPath, string toPath);
    bool remove(string path);
    string combine(string basePath, string localPath);
    FileHandle open(string path, string mode);
    std::vector<string> find(string wildcard);
    string getDir(string path);
    std::vector<string> complete(string partialName, string path, bool includeFiles = true, bool includeSlashes = false);
};

extern FSAPI fs;

#endif