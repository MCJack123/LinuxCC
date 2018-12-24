#ifndef IOAPI_HPP
#define IOAPI_HPP
#include <string>
#include <vector>
#include <stdio.h>
#include "iterator.hpp"

using std::string;

class IOHandleIterator : Iterator<string> {
    friend class IOHandle;
};

class IOHandle {
    friend class IOAPI;
    FILE * fp;
    string mode;
    string path;
    IOHandle(FILE * file, string mode): fp(file), mode(mode) {}
public:
    IOHandle(string path, string mode);
    ~IOHandle();
    void close();
    void flush();
    IOHandleIterator lines();
    string read(string format = "*l");
    void write(string v);
    void write(int n);
    void write(double n);
};

class IOAPI {
    IOHandle input = IOHandle(stdin, "r");
    IOHandle output = IOHandle(stdout, "w");
public:
    void close(IOHandle file) {file.close();}
    void close() {output.close();}
    void flush() {input.flush();}
    IOHandle input() {return input;}
    void input(string path) {input = IOHandle(path, "r");}
    void input(IOHandle file) {input = file;}
    IOHandleIterator lines() {return input.lines();}
    IOHandleIterator lines(string path) {return IOHandle(path, "r").lines();}
    IOHandle open(string path, string mode = "r") {return IOHandle(path, mode);}
    IOHandle output() {return output;}
    void output(string path) {output = IOHandle(path, "w");}
    void output(IOHandle file) {output = file;}
    string read(string format = "*l") {return input.read();}
    string type(IOHandle obj);
    void write(string v) {output.write(v);}
    void write(int n) {output.write(n);}
    void write(double n) {output.write(n);}
};

extern IOAPI io;

#endif