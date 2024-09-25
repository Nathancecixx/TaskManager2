cmake_minimum_required(VERSION 3.26)
project(TaskManager2)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include FetchContent module to fetch wxWidgets
include(FetchContent)

# Fetch wxWidgets from GitHub
FetchContent_Declare(
        wxWidgets
        GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
        GIT_TAG        v3.2.2.1 # You can specify a stable version or tag
)

# Make wxWidgets available
FetchContent_MakeAvailable(wxWidgets)

# Include wxWidgets definitions and directories
include(${wxWidgets_USE_FILE})

# Add the executable with your source files
add_executable(TaskManager2
        main.cpp
        ProcessManager.cpp
        FileParser.cpp
        ProcessManager.h
        Process.h
        FileParser.h
)

# Link the fetched wxWidgets libraries to the executable
target_link_libraries(TaskManager2 ${wxWidgets_LIBRARIES})

# Platform-specific linking (for Windows-specific libraries)
if(WIN32)
    target_link_libraries(TaskManager2
            wbemuuid
            pdh
            ole32
            oleaut32
            uuid
            psapi
    )
endif()

# Install rules (optional, helps with setup if you want to distribute the project)
install(TARGETS TaskManager2 DESTINATION bin)
