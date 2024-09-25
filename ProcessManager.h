
#ifndef TASKMANAGER2_PROCESSMANAGER_H
#define TASKMANAGER2_PROCESSMANAGER_H

#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <Wbemidl.h>
#include <comdef.h>
#include <Pdh.h>
#include <psapi.h>

#endif

#ifdef __linux__
#include <dirent.h>
#include <fstream>
#include <sstream>

#include "FileParser.h"

#endif


#include "Process.h"
using namespace std;


typedef struct {
    string cpu;
    vector<string> cores;

    //CPU usage
    string userTime;
    string systemTime;
    string idleTime;
    string ioWaitTime;

    //Processes
    string totalProcessCount;
    string processRunningCount;
    string processBlocked;

    string contextSwitching;

    string bootTime;

} PROCESSING_INFO;

typedef struct {
    string memTotal;
    string memFree;
    string memAvailable;
    string swapMemUsage;
} MEMORY_INFO;


class ProcessManager {
private:
    vector<PROCESS> processList;
    PROCESSING_INFO cpuInfo;
    MEMORY_INFO memoryInfo;
    FileParser fileParser;


public:
    ProcessManager();

    int PrintProcesses();

    int ReloadProcInfo();
    int ReloadCpuInfo();
    int ReloadMemInfo();


};



#endif //TASKMANAGER2_PROCESSMANAGER_H
