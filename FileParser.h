
#ifndef TASKMANAGER2_FILEPARSER_H
#define TASKMANAGER2_FILEPARSER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "Process.h"


class FileParser{
private:
    int parseStatFile(const std::string& pid, PROCESS& proc);

    int parseStatusFile(const std::string& pid, PROCESS& proc);

    int parseCmdlineFile(const std::string& pid, PROCESS& proc);

    int parseIOFile(const std::string& pid, PROCESS& proc);

public:
    int populateProcessInfo(const std::string& pid, PROCESS& proc);

};



#endif //TASKMANAGER2_FILEPARSER_H
