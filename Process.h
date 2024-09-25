
#ifndef TASKMANAGER2_PROCESS_H
#define TASKMANAGER2_PROCESS_H

#include <string>

typedef struct {
    std::string id;                   // Process ID
    std::string name;                 // Command name
    char state;                       // Process state
    std::string ppid;                 // Parent process ID
    long utime;                       // User mode time (clock ticks)
    long stime;                       // Kernel mode time (clock ticks)
    long cutime;                      // Children's user mode time (clock ticks)
    long cstime;                      // Children's kernel mode time (clock ticks)
    long priority;                    // Priority value
    long nice;                        // Nice value
    long num_threads;                 // Number of threads
    unsigned long long starttime;     // Start time after system boot (clock ticks)
    unsigned long vsize;              // Virtual memory size (bytes)
    long rss;                         // Resident Set Size (pages)
    std::string tty_nr;               // Controlling terminal
    std::string uid;                  // User ID
    std::string gid;                  // Group ID
    std::string cmdline;              // Full command line
    unsigned long read_bytes;         // Bytes read (from /proc/[pid]/io)
    unsigned long write_bytes;        // Bytes written (from /proc/[pid]/io)
} PROCESS;

#endif //TASKMANAGER2_PROCESS_H
