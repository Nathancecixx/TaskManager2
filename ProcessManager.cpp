#include "ProcessManager.h"


#ifdef _WIN32

#include <string>
#include <initguid.h>

ProcessManager::ProcessManager() {
    this->ReloadCpuInfo();
}

std::string BSTRToString(BSTR bstr) {
    if (bstr == NULL) {
        return "";
    }
    // Get the length of the BSTR
    int wslen = SysStringLen(bstr);
    // Determine the required buffer size for the converted string
    int len = WideCharToMultiByte(CP_UTF8, 0, bstr, wslen, NULL, 0, NULL, NULL);
    // Allocate a string to hold the converted characters
    std::string str(len, '\0');
    // Perform the conversion
    WideCharToMultiByte(CP_UTF8, 0, bstr, wslen, &str[0], len, NULL, NULL);
    return str;
}

int ProcessManager::ReloadCpuInfo() {
    HRESULT hres;

    // Initialize COM library
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        return 1;
    }

    // Set general COM security levels
    hres = CoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities
            NULL                         // Reserved
    );

    if (FAILED(hres)) {
        CoUninitialize();
        return 1;
    }

    // Obtain the initial locator to WMI
    IWbemLocator* pLoc = NULL;

    hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres)) {
        CoUninitialize();
        return 1;
    }

    // Connect to WMI namespace
    IWbemServices* pSvc = NULL;

    BSTR namespaceString = SysAllocString(L"ROOT\\CIMV2");
    hres = pLoc->ConnectServer(
            namespaceString,         // WMI namespace
            NULL,                    // User name
            NULL,                    // User password
            NULL,                    // Locale
            0,                       // Security flags (should be LONG)
            NULL,                    // Authority
            NULL,                    // Context object
            &pSvc                    // IWbemServices proxy
    );
    SysFreeString(namespaceString);

    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // Set security levels on the proxy
    hres = CoSetProxyBlanket(
            pSvc,                        // The proxy to set
            RPC_C_AUTHN_WINNT,           // Authentication service
            RPC_C_AUTHZ_NONE,            // Authorization service
            NULL,                        // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,      // Authentication level
            RPC_C_IMP_LEVEL_IMPERSONATE, // Impersonation level
            NULL,                        // Client identity
            EOAC_NONE                    // Proxy capabilities
    );

    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // Query CPU information
    IEnumWbemClassObject* pEnumerator = NULL;
    BSTR queryLanguage = SysAllocString(L"WQL");
    BSTR query = SysAllocString(L"SELECT Name, NumberOfCores FROM Win32_Processor");
    hres = pSvc->ExecQuery(
            queryLanguage,
            query,
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);
    SysFreeString(queryLanguage);
    SysFreeString(query);

    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // Get CPU name and cores
    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                                       &pclsObj, &uReturn);

        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;

        // CPU Name
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            cpuInfo.cpu = BSTRToString(vtProp.bstrVal);
            VariantClear(&vtProp);
        }

        // Number of Cores
        hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            int numCores = vtProp.intVal;
            VariantClear(&vtProp);

            for (int i = 0; i < numCores; i++) {
                cpuInfo.cores.push_back("Core " + std::to_string(i + 1));
            }
        }

        pclsObj->Release();
    }

    // Get system times
    FILETIME idleTime, kernelTime, userTime;
    BOOL res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    if (res) {
        ULONGLONG idle = (((ULONGLONG)idleTime.dwHighDateTime) << 32) | idleTime.dwLowDateTime;
        ULONGLONG kernel = (((ULONGLONG)kernelTime.dwHighDateTime) << 32) | kernelTime.dwLowDateTime;
        ULONGLONG user = (((ULONGLONG)userTime.dwHighDateTime) << 32) | userTime.dwLowDateTime;

        ULONGLONG actualKernel = kernel - idle;

        cpuInfo.userTime = std::to_string(user);
        cpuInfo.systemTime = std::to_string(actualKernel);
        cpuInfo.idleTime = std::to_string(idle);
        cpuInfo.ioWaitTime = "N/A"; // Not directly available on Windows
    }

    // Get total process count
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
        cProcesses = cbNeeded / sizeof(DWORD);
        cpuInfo.totalProcessCount = std::to_string(cProcesses);
    }

    // Process running count (simplified as total processes)
    cpuInfo.processRunningCount = cpuInfo.totalProcessCount;

    // Process blocked (not directly available)
    cpuInfo.processBlocked = "N/A";

    // Get context switching rate
    PDH_HQUERY hQuery;
    PDH_HCOUNTER hCounter;
    PDH_FMT_COUNTERVALUE counterVal;

    PDH_STATUS pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);
    if (pdhStatus == ERROR_SUCCESS) {
        pdhStatus = PdhAddCounterW(hQuery, L"\\System\\Context Switches/sec", 0, &hCounter);
        if (pdhStatus == ERROR_SUCCESS) {
            PdhCollectQueryData(hQuery);
            Sleep(1000); // Wait to collect data
            pdhStatus = PdhCollectQueryData(hQuery);
            if (pdhStatus == ERROR_SUCCESS) {
                pdhStatus = PdhGetFormattedCounterValue(hCounter, PDH_FMT_DOUBLE, NULL, &counterVal);
                if (pdhStatus == ERROR_SUCCESS) {
                    cpuInfo.contextSwitching = std::to_string(counterVal.doubleValue);
                }
            }
        }
        PdhCloseQuery(hQuery);
    }

    // Get boot time
    pEnumerator = NULL;
    queryLanguage = SysAllocString(L"WQL");
    query = SysAllocString(L"SELECT LastBootUpTime FROM Win32_OperatingSystem");
    hres = pSvc->ExecQuery(
            queryLanguage,
            query,
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);
    SysFreeString(queryLanguage);
    SysFreeString(query);

    if (SUCCEEDED(hres)) {
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                                           &pclsObj, &uReturn);

            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;

            hr = pclsObj->Get(L"LastBootUpTime", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr)) {
                cpuInfo.cpu = BSTRToString(vtProp.bstrVal);
                VariantClear(&vtProp);
            }

            pclsObj->Release();
        }
    }

    // Cleanup
    if (pEnumerator) pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return 0;
}

int ProcessManager::ReloadMemInfo() {
    // Implement memory info retrieval
    return 0;
}

int ProcessManager::ReloadProcInfo() {
    // Implement process info retrieval
    return 0;
}

int ProcessManager::PrintProcesses() {
    cout << "CPU User Time: " << cpuInfo.userTime << endl;
    cout << "CPU System Time: " << cpuInfo.systemTime << endl;
    cout << "CPU Idle Time: " << cpuInfo.idleTime << endl;
    cout << "CPU IO Wait Time: " << cpuInfo.ioWaitTime << endl;
    cout << "Context Switches: " << cpuInfo.contextSwitching << endl;
    cout << "Boot Time: " << cpuInfo.bootTime << endl;
    cout << "Total Processes: " << cpuInfo.totalProcessCount << endl;
    cout << "Running Processes: " << cpuInfo.processRunningCount << endl;
    cout << "Blocked Processes: " << cpuInfo.processBlocked << endl;
    return 0;
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