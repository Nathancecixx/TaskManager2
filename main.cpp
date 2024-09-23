#include <iostream>
#include <windows.h>
#include <psapi.h>

std::string GetProcessName(DWORD processID) {
    // Get a handle to the process.

    HANDLE processHandle = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );


    std::string processName("Unknown Process");
    if (processHandle != NULL)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( processHandle,
                                 &hMod,
                                 sizeof(hMod),
                                 &cbNeeded) )
        {
            char nameBuffer[MAX_PATH];
            if(GetModuleBaseNameA( processHandle,
                               hMod,
                               nameBuffer,
                               sizeof(processName)/sizeof(char)))
            {
                processName = nameBuffer;
            }
        }
    }

    CloseHandle( processHandle );
    return processName;
}


int main() {

    DWORD processes [1024], cbNeeded, processCount;

    if(!EnumProcesses(processes, sizeof(processes), &cbNeeded))
        return 1;


    processCount = cbNeeded/sizeof(DWORD);

    for(int i = 0; i < processCount; i++){
        std::cout << processes[i] << "  ====  " << GetProcessName(processes[i]) << "\n";
    }


    return 0;
}
