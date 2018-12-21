#if !defined(_GNU_SOURCE) && _POSIX_C_SOURCE < 200809L
#error This program uses GNU extensions. Please recompile with GCC.
#endif
#include "fs.hpp"
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

FSAPI fs;

FileHandle::FileHandle(string path, string mode) {
    this->mode = mode;
    this->path = path;
    fp = fopen(path.c_str(), mode.c_str());
    if (fp == nullptr) throw "Could not open file: " + errno;
}

FileHandle::~FileHandle() {
    if (fp != NULL) fclose(fp);
}

void FileHandle::close() {
    if (fp != NULL) fclose(fp);
    fp = NULL;
}

string FileHandle::readLine() {
    if (fp == NULL || mode.find('r') == string::npos || mode.find('b') != string::npos) return "";
    char * line = NULL;
    if (getline(&line, NULL, fp) == -1) return "";
    string retval(line);
    free(line);
    return retval;
}

string FileHandle::readAll() {
    if (fp == NULL || mode.find('r') == string::npos || mode.find('b') != string::npos) return "";
    int len = fs.getSize(path) - ftell(fp);
    if (len < 1) return "";
    char * buf = (char*)malloc(len);
    fread(buf, len, 1, fp);
    string retval(buf);
    free(buf);
    return retval;
}

void FileHandle::write(string data) {
    if (fp == NULL || mode.find('w') == string::npos || mode.find('b') != string::npos) return;
    fwrite(data.c_str(), data.size(), 1, fp);
}

void FileHandle::writeLine(string data) {
    if (fp == NULL || mode.find('w') == string::npos || mode.find('b') != string::npos) return;
    fwrite(data.c_str(), data.size(), 1, fp);
    fwrite("\n", 1, 1, fp);
}

void FileHandle::flush() {
    if (!(fp == NULL || mode.find('w') == string::npos)) fflush(fp);
}

char FileHandle::read() {
    if (fp == NULL || mode.find('r') == string::npos || mode.find('b') == string::npos) return 0;
    return fgetc(fp);
}

void FileHandle::write(char byte) {
    if (!(fp == NULL || mode.find('w') == string::npos || mode.find('b') == string::npos)) fputc(byte, fp);
}

std::vector<string> FSAPI::list(string path) {
    std::vector<string> retval;
    DIR * d;
    struct dirent * dir;
    d = opendir(path.c_str());
    if (!d) return retval;
    while ((dir = readdir(d)) != NULL) retval.push_back(string(dir->d_name));
    closedir(d);
    return retval;
}

bool FSAPI::exists(string path) {
    return access(path.c_str(), F_OK) != -1;
}

bool FSAPI::isDir(string path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

bool FSAPI::isReadOnly(string path) {
    return access(path.c_str(), W_OK) == -1;
}

string FSAPI::getName(string path) {
    char * str = (char*)malloc(path.size() + 1);
    strcpy(str, path.c_str());
    string retval(basename(str));
    free(str);
    return retval;
}

string FSAPI::getDrive(string path) {
    char *test_path = (char*)malloc(PATH_MAX), *test_end;
    struct stat cur_stat, prev_stat;
    char * mount_point;

    if (lstat(path.c_str(), &prev_stat) < 0)
        return "";

    test_end = stpcpy(test_path, path.c_str());
    if (!S_ISDIR(prev_stat.st_mode)) {
        test_end = strrchr(test_path, '/');
        if (test_end == NULL)
            test_end = stpcpy(test_path, ".");
        else
            *test_end = '\0';
    }

    for (;;) {
        test_end = stpcpy(test_end, "/..");     
        if (lstat(test_path, &cur_stat) < 0)
            return "";
        if (cur_stat.st_dev != prev_stat.st_dev || cur_stat.st_ino == prev_stat.st_ino) /* root */
            break; /* this is the mount point */
        prev_stat = cur_stat;
    }

    *(test_end - 3) = '\0';
    if (realpath(test_path, mount_point) == NULL) {
        free(test_path);
        return "";
    }
    string retval(mount_point);
    free(mount_point);
    return retval;
}

int FSAPI::getSize(string path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    return path_stat.st_size;
}

int FSAPI::getFreeSpace(string path) {
    struct statvfs fs_stat;
    statvfs(path.c_str(), &fs_stat);
    return fs_stat.f_bsize * fs_stat.f_bfree;
}

bool FSAPI::makeDir(string path) {
    return mkdir(path.c_str(), 0777) == 0;
}

bool FSAPI::move(string fromPath, string toPath) {
    return rename(fromPath.c_str(), toPath.c_str()) == 0;
}

bool FSAPI::copy(string fromPath, string toPath) {
    FILE * in = fopen(fromPath.c_str(), "r"), * out = fopen(toPath.c_str(), "w");
    if (in == NULL || out == NULL) return false;
    char * buf = (char*)malloc(16384);
    int read = 0;
    while ((read = fread(buf, 16384, 1, in)) == 16384) fwrite(buf, read, 1, out);
    fwrite(buf, read, 1, out);
    fclose(in);
    fclose(out);
    return true;
}

bool FSAPI::remove(string path) {
    if (isDir(path)) {
        std::vector<string> files = list(path);
        for (string f : files) if (!remove(combine(path, f))) return false;
    } else return remove(path.c_str()) == 0;
}

string FSAPI::combine(string basePath, string localPath) {
    if (basePath[basePath.size()-1] == '/') basePath = basePath.substr(0, basePath.size()-1);
    if (localPath[0] == '/') localPath = localPath.substr(1);
    return basePath + "/" + localPath;
}

FileHandle FSAPI::open(string path, string mode) {
    return FileHandle(path, mode);
}

std::vector<string> FSAPI::find(string wildcard) {
    std::vector<string> retval;
    if (wildcard.find_first_of(";&|") != std::string::npos) return retval;
    FILE * pipe = popen(("find " + wildcard).c_str(), "r");
    char * line = NULL;
    while (getline(&line, NULL, pipe) != -1) {
        retval.push_back(string(line));
        free(line);
    }
    pclose(pipe);
    return retval;
}

string FSAPI::getDir(string path) {
    char * str = (char*)malloc(path.size() + 1);
    strcpy(str, path.c_str());
    string retval(dirname(str));
    free(str);
    return retval;
}

std::vector<string> FSAPI::complete(string partialName, string path, bool includeFiles, bool includeSlashes) {
    std::vector<string> files = list(path), retval;
    for (string f : files) 
        if (f.substr(0, partialName.size()) == partialName && (!includeFiles && !isDir(combine(path, f)))) 
            retval.push_back(f.substr(partialName.size()) + (includeSlashes ? "/" : ""));
    return retval;
}