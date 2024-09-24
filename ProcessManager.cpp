
#include "ProcessManager.h"


#ifdef _WIN32
ProcessManager::ProcessManager() {}

int ProcessManager::ReloadCpuInfo() {}

int ProcessManager::ReloadMemInfo() {}

int ProcessManager::ReloadProcInfo() {}

int ProcessManager::PrintProcesses(){


}


#endif
#ifdef __linux__

ProcessManager::ProcessManager() {
    this->ReloadMemInfo();
    this->ReloadCpuInfo();
    this->ReloadProcInfo();
}


int ProcessManager::ReloadCpuInfo() {
    string fileLocation("/proc/stat");
    ifstream file(fileLocation);
    string line;

    while (getline(file, line)) {
        // Create a stringstream to parse the line
        stringstream ss(line);
        string label;
        ss >> label;  // Get the label (e.g., "cpu", "cpu0", "ctxt", etc.)

        // Check if the line starts with "cpu" for CPU data
        if (label.rfind("cpu", 0) == 0) {
            string user, nice, system, idle, ioWait;
            ss >> user >> nice >> system >> idle >> ioWait;

            if (label == "cpu") {
                cpuInfo.userTime = user;
                cpuInfo.systemTime = system;
                cpuInfo.idleTime = idle;
                cpuInfo.ioWaitTime = ioWait;
            } else {
                cpuInfo.cores.push_back(line);
            }
        }
        else if (label == "ctxt") {
            ss >> cpuInfo.contextSwitching;
        }
        else if (label == "btime") {
            ss >> cpuInfo.bootTime;
        }
        else if (label == "processes") {
            ss >> cpuInfo.totalProcessCount;
        }
        else if (label == "procs_running") {
            ss >> cpuInfo.processRunningCount;
        }
        else if (label == "procs_blocked") {
            ss >> cpuInfo.processBlocked;
        }
    }

    file.close();

    return 0;
}

int ProcessManager::ReloadMemInfo() {
    string fileLocation("/proc/meminfo");
    ifstream file(fileLocation);
    string line;

    // Read each line from the file
    while (getline(file, line)) {
        stringstream ss(line);
        string label;
        ss >> label;  // Get the label (e.g., "MemTotal:", "MemFree:", etc.)

        // Remove the colon from the label
        label = label.substr(0, label.size() - 1);

        // Check the labels and extract values
        if (label == "MemTotal") {
            ss >> memoryInfo.memTotal;
        } else if (label == "MemFree") {
            ss >> memoryInfo.memFree;
        } else if (label == "MemAvailable") {
            ss >> memoryInfo.memAvailable;
        } else if (label == "SwapTotal") {
            string swapTotal;
            ss >> swapTotal;
            memoryInfo.swapMemUsage = swapTotal;
        }
    }

    file.close();

    return 0;
}

int ProcessManager::ReloadProcInfo() {
    struct dirent* pDirent;
    DIR* pDir = opendir("/proc");
    if(pDir == NULL){
        std::cout << "Failed to open process folder..." << std::endl;
        return 1;
    }

    while( (pDirent = readdir(pDir)) != NULL){
        if( isdigit(pDirent->d_name[0]) ){

            std::string fileLocation("/proc//comm");
            std::string procId(pDirent->d_name);

            fileLocation.insert(6, procId);

            std::string processName;

            std::ifstream processFile(fileLocation);
            getline(processFile, processName);

            PROCESS currentProcess;
            currentProcess.id = pDirent->d_name;
            currentProcess.name =processName;
            processList.push_back(currentProcess);
        }
    }

    closedir(pDir);
    return 0;
};


int ProcessManager::PrintProcesses(){

    cout << "Memory Total: " << memoryInfo.memTotal << endl;
    cout << "Memory Free: " << memoryInfo.memFree << endl;
    cout << "Memory Available: " << memoryInfo.memAvailable << endl;
    cout << "Swap Memory Total: " << memoryInfo.swapMemUsage << endl;


    cout << "CPU User Time: " << cpuInfo.userTime << endl;
    cout << "CPU System Time: " << cpuInfo.systemTime << endl;
    cout << "CPU Idle Time: " << cpuInfo.idleTime << endl;
    cout << "CPU IO Wait Time: " << cpuInfo.ioWaitTime << endl;
    cout << "Context Switches: " << cpuInfo.contextSwitching << endl;
    cout << "Boot Time: " << cpuInfo.bootTime << endl;
    cout << "Total Processes: " << cpuInfo.totalProcessCount << endl;
    cout << "Running Processes: " << cpuInfo.processRunningCount << endl;
    cout << "Blocked Processes: " << cpuInfo.processBlocked << endl;


    for(int i = 0; i < processList.size(); i++){
        cout << processList.at(i).id << ": " << processList.at(i).name << endl;
    }


    return 0;
}

#endif