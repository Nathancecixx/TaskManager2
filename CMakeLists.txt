cmake_minimum_required(VERSION 3.26)
project(TaskManager2)

set(CMAKE_CXX_STANDARD 17)

add_executable(TaskManager2 main.cpp
        ProcessManager.cpp
        ProcessManager.h)

# Link against required libraries
if(MINGW)
    target_link_libraries(TaskManager2
            wbemuuid
            pdh
            ole32
            oleaut32
            uuid
            psapi
    )
endif()