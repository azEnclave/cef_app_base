#include "main_util.hpp"

namespace shared
{
    // Helper utility method for creating a commandLine object that initializes from the arguments
    CefRefPtr<CefCommandLine> createCommandLine(const CefMainArgs &mainArgs)
    {
        CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
        command_line->InitFromString(::GetCommandLineW());
#else
        command_line->InitFromArgv(mainArgs.argc, mainArgs.argv);
#endif
        return command_line;
    }

    ProcessType getProcessType(const CefRefPtr<CefCommandLine> &cefCommandLine)
    {
        ProcessTypeName processTypeName;

        if (!cefCommandLine->HasSwitch(processTypeName.processTypeSwitch))
            return PROCESS_TYPE_BROWSER;

        const std::string& process_type = cefCommandLine->GetSwitchValue(processTypeName.processTypeSwitch);
        if (process_type == processTypeName.renderProcess)
            return PROCESS_TYPE_RENDERER;

#if defined(OS_LINUX)
        // On Linux the zygote process is used to spawn other process types. Since we
        // don't know what type of process it will be we give it the renderer app.
        if (process_type == processTypeName.zygoteProcess)
            return PROCESS_TYPE_OTHER;
#endif

        return PROCESS_TYPE_OTHER;
    }

}