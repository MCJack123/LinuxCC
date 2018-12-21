#include "disk.hpp"
#include "commands.hpp"
#include <unistd.h>
#include <regex>
#include <fstream>
#include <sys/stat.h>

const std::regex node_regex("^(fd|ram|scd|[sh]d[a-z]|sr)\d$|^(mmcblk|loop)\dp\d$|^cdrom$");
const std::regex path_regex("^(\/\w+)+$");

bool isClean(string node) {
    return std::regex_match(node, node_regex) || std::regex_match(node, path_regex);
}

bool DiskAPI::isPresent(string node) {
    if (!isClean(node)) return false;
    return access(("/dev/" + node).c_str(), F_OK) != -1;
}

bool DiskAPI::isMounted(string node) {
    if (!isClean(node)) return false;
    std::ifstream in("/proc/mounts");
    string str;
    std::regex nmatch("/dev/" + node);
    while (std::getline(in, str)) if (std::regex_match(str, nmatch)) {in.close(); return true;}
    in.close();
    return false;
}

int DiskAPI::mount(string node, string mountPoint) {
    if (!isClean(node) || !isClean(mountPoint)) return -1;
    if (getuid() != 0) return -2;
    return commands.exec("mount '" + node + "' '" + mountPoint + "'");
}

int DiskAPI::unmount(string mountPoint) {
    if (!isClean(mountPoint)) return -1;
    if (getuid() != 0) return -2;
    return commands.exec("umount '" + mountPoint + "'");
}

string DiskAPI::getMountPath(string node) {
    if (!isClean(node)) return "";
    std::ifstream in("/proc/mounts");
    string str;
    std::regex nmatch("/dev/" + node);
    while (std::getline(in, str)) if (std::regex_match(str, nmatch)) {
        std::smatch match;
        std::regex_search(str, match, std::regex("/dev/" + node + " on (\/\w+)+"));
        in.close();
        return ((std::string)match[0]).substr(strlen(("/dev/" + node + " on ").c_str()));
    }
    in.close();
    return "";
}

string DiskAPI::getNode(string mountPoint) {
    if (!isClean(mountPoint)) return "";
    std::ifstream in("/proc/mounts");
    string str;
    std::regex nmatch(mountPoint);
    while (std::getline(in, str)) if (std::regex_match(str, nmatch)) {
        in.close();
        return str.substr(0, str.find(' '));
    }
    in.close();
    return "";
}

bool DiskAPI::hasAudio(string node) {
    if (!isClean(node)) return false;
    // this isn't really a great way to check since it returns true if a disc is failing,
    // but it will always return true if the disc has audio (dd can't read CDDA's)
    return commands.exec("dd bs=1 count=1 'if=/dev/" + node + "' 2>&1 > /dev/null");
}

int DiskAPI::eject(string node) {
    if (!isClean(node)) return -1;
    if (getuid() != 0) return -2;
    return commands.exec("eject '/dev/" + node + "'");
}

DiskAPI disk;