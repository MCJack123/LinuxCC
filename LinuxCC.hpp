#ifndef LINUXCC_HPP
#define LINUXCC_HPP

#include "bit.hpp"
#include "colors.hpp"
#include "commands.hpp"
#include "coroutine.hpp"
#include "disk.hpp"
#include "fs.hpp"
#include "help.hpp"
#include "http.hpp"
#include "io.hpp"
#include "iterator.hpp"
#include "math.hpp"
#include "os.hpp"
#include "parallel.hpp"
#include "settings.hpp"
#include "term.hpp"
#include "textutils.hpp"
#include "window.hpp"

#ifndef LINUXCC_NO_CCMAIN

int CCmain(int argc, const char * argv[]);

int main(int argc, const char * argv[]) {
    pid_t PID = fork();
    if (PID == -1) {
        printf("Error: Couldn't fork process\n");
        return 1;
    } else if (PID == 0) os.runLoop();
    else return CCmain(argc, argv);
    return 0;
}

#endif

#endif