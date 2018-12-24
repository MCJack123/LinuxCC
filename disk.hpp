#ifndef DISKAPI_HPP
#define DISKAPI_HPP
#include <string>

using std::string;

class DiskAPI {
public:
    bool isPresent(string node);
    bool isMounted(string node);
    int mount(string node, string mountPoint);
    int unmount(string mountPoint);
    string getMountPath(string node);
    string getNode(string mountPoint);
    bool hasAudio(string node);
    int eject(string node);
};

extern DiskAPI disk;

#endif