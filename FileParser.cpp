#include "FileParser.h"

int FileParser::parseStatFile(const std::string& pid, PROCESS& proc) {
    std::string statPath = "/proc/" + pid + "/stat";
    std::ifstream statFile(statPath);
    if (!statFile.is_open()) {
        return 1;
    }

    std::string line;
    std::getline(statFile, line);
    statFile.close();

    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;

    // Parse PID
    iss >> proc.id;

    // Parse process name (which may contain spaces and is enclosed in parentheses)
    iss >> token;
    proc.name = token.substr(1, token.length() - 2); // Remove parentheses

    // Parse remaining fields
    iss >> proc.state;
    iss >> proc.ppid;
    iss >> token; // pgrp
    iss >> token; // session
    iss >> proc.tty_nr;
    // Skip unused fields
    for (int i = 0; i < 6; ++i) iss >> token;

    iss >> proc.utime;
    iss >> proc.stime;
    iss >> proc.cutime;
    iss >> proc.cstime;
    iss >> proc.priority;
    iss >> proc.nice;
    iss >> proc.num_threads;
    iss >> token; // itrealvalue
    iss >> proc.starttime;
    iss >> proc.vsize;
    iss >> proc.rss;

    return 0;
}

int FileParser::parseStatusFile(const std::string& pid, PROCESS& proc){
    std::string statusPath = "/proc/" + pid + "/status";
    std::ifstream statusFile(statusPath);
    if (!statusFile.is_open()) {
        // Handle error
        return 1;
    }

    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.compare(0, 4, "Uid:") == 0) {
            std::istringstream iss(line.substr(5));
            iss >> proc.uid;
        } else if (line.compare(0, 4, "Gid:") == 0) {
            std::istringstream iss(line.substr(5));
            iss >> proc.gid;
        } else if (line.compare(0, 7, "Threads") == 0) {
            std::istringstream iss(line.substr(8));
            iss >> proc.num_threads;
        }
    }
    statusFile.close();
    return 0;
}

int FileParser::parseCmdlineFile(const std::string& pid, PROCESS& proc){
    std::string cmdlinePath = "/proc/" + pid + "/cmdline";
    std::ifstream cmdlineFile(cmdlinePath, std::ios::binary);
    if (!cmdlineFile.is_open()) {
        // Handle error
        return 1;
    }

    std::string cmdline;
    std::getline(cmdlineFile, cmdline, '\0'); // Read until null character
    cmdlineFile.close();

    // Replace null characters with spaces to separate arguments
    std::replace(cmdline.begin(), cmdline.end(), '\0', ' ');

    proc.cmdline = cmdline;
    return 0;
}

int FileParser::parseIOFile(const std::string& pid, PROCESS& proc){
    std::string ioPath = "/proc/" + pid + "/io";
    std::ifstream ioFile(ioPath);
    if (!ioFile.is_open()) {
        // Handle error
        return 1;
    }

    std::string line;
    while (std::getline(ioFile, line)) {
        if (line.compare(0, 10, "read_bytes") == 0) {
            std::istringstream iss(line.substr(12));
            iss >> proc.read_bytes;
        } else if (line.compare(0, 11, "write_bytes") == 0) {
            std::istringstream iss(line.substr(13));
            iss >> proc.write_bytes;
        }
        // Parse additional fields as needed
    }
    ioFile.close();
    return 0;
}

int FileParser::populateProcessInfo(const std::string &pid, PROCESS &proc) {
    if(parseStatFile(pid,proc))
        return 1;

    if(parseStatusFile(pid,proc))
        return 1;

    if(parseCmdlineFile(pid,proc))
        return 1;

    if(parseIOFile(pid,proc))
        return 1;


    return 0;
}
