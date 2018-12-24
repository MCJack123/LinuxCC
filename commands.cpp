#include <unistd.h>
#include "commands.hpp"
#include "os.hpp"

int CommandsAPI::exec(string command) {
    char *const argv[2] = {"-c", (char *const)command.c_str()};
    return execv("/bin/bash", argv);
}

int CommandsAPI::execAsync(string command) {
    pid_t PID = fork();
    if (PID == 0) {
        char *const argv[2] = {"-c", (char *const)command.c_str()};
        int retval = execv("/bin/bash", argv);
        os.queueEvent(event_t("task_complete").setInt(0, getpid()).setInt(1, retval));
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