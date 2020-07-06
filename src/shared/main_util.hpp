#ifndef CUSTOM_CEF_APP_MAIN_UTIL_H
#define CUSTOM_CEF_APP_MAIN_UTIL_H

#include <include/cef_command_line.h>

namespace shared
{
    // Process types that may have different CefApp instances.
    enum ProcessType
    {
        PROCESS_TYPE_BROWSER,
        PROCESS_TYPE_RENDERER,
        PROCESS_TYPE_OTHER,
    };

    // Store all possible process type names
    struct ProcessTypeName
    {
        const std::string zygoteProcess = "zygote";
        const std::string renderProcess = "renderer";
        const std::string utilityProcess = "utility";
        const std::string processTypeSwitch = "type";
    };

    CefRefPtr<CefCommandLine> createCommandLine(const CefMainArgs &mainArgs);

    // Determine the process type based on command-line arguments.
    ProcessType getProcessType(const CefRefPtr<CefCommandLine> &cefCommandLine);
}

#endif
