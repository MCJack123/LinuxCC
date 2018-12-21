#include <string>

using std::string;

class CommandsAPI {
public:
    int exec(string command);
    int execAsync(string command);
    string pexec(string command);
};

extern CommandsAPI commands;