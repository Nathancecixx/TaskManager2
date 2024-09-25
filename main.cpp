#include <iostream>

#ifdef _WIN32
//#include <windows.h>
//#include <psapi.h>
//
//std::string GetProcessName(DWORD processID) {
//    // Get a handle to the process.
//
//    HANDLE processHandle = OpenProcess( PROCESS_QUERY_INFORMATION |
//                                        PROCESS_VM_READ,
//                                        FALSE, processID );
//
//
//    std::string processName("Unknown Process");
//    if (processHandle != NULL)
//    {
//        HMODULE hMod;
//        DWORD cbNeeded;
//
//        if ( EnumProcessModules( processHandle,
//                                 &hMod,
//                                 sizeof(hMod),
//                                 &cbNeeded) )
//        {
//            char nameBuffer[MAX_PATH];
//            if(GetModuleBaseNameA( processHandle,
//                                   hMod,
//                                   nameBuffer,
//                                   sizeof(processName)/sizeof(char)))
//            {
//                processName = nameBuffer;
//            }
//        }
//    }
//
//    CloseHandle( processHandle );
//    return processName;
//}
//
//
//int main() {
//
//    DWORD processes [1024], cbNeeded, processCount;
//
//    if(!EnumProcesses(processes, sizeof(processes), &cbNeeded))
//        return 1;
//
//
//    processCount = cbNeeded/sizeof(DWORD);
//
//    for(int i = 0; i < processCount; i++){
//        std::cout << processes[i] << "  ====  " << GetProcessName(processes[i]) << "\n";
//    }
//
//
//    return 0;
//
//}

//#include "ProcessManager.h"
//
//
//
//int main() {
//
//    ProcessManager pm = ProcessManager();
//
//    pm.PrintProcesses();
//    return 0;
//}

// Start of wxWidgets "Hello World" Program
#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}


#endif

#ifdef __linux__

// Start of wxWidgets "Hello World" Program
#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Hello = 1
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}


//#include <ftw.h>
//#include <fnmatch.h>
//
//#include "ProcessManager.h"
//#include "AppManager.h"
//
//
//int main(int argc, char** argv) {
//
//    ProcessManager pm = ProcessManager();
//
//    pm.PrintProcesses();
//
//    AppManager appManager(argc, argv);
//    return appManager.Run();
//}

#endif
