#include <unistd.h>
#include "commands.hpp"

int CommandsAPI::exec(string command) {
    char *const argv[2] = {"-c", (char *const)command.c_str()};
    return execv("/bin/bash", argv);
}

int CommandsAPI::execAsync(string command) {
    pid_t PID = fork();
    if (PID == 0) {
        char *const argv[2] = {"-c", (char *const)command.c_str()};
        int retval = execv("/bin/bash", argv);
        // do something with the return value
    } else return PID;
}

string CommandsAPI::pexec(string command) {
    string retval;
    char buf[512];
    int r = 0;
    FILE * pipe = popen(command.c_str(), "r");
    while ((r = fread(buf, 512, 1, pipe)) == 512) retval += string(buf, r);
    retval += string(buf, r);
    pclose(pipe);
    return retval;
}

CommandsAPI commands;