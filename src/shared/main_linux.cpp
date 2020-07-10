#include <X11/Xlib.h>
#include <include/cef_command_line.h>
#include <include/cef_app.h>
#include <iostream>

#include "main.hpp"
#include "main_util.hpp"
#include "app_factory.hpp"
#include "client_manager.hpp"
#include <src/main/applicationSettings.hpp>

namespace shared
{
	int XErrorHandlerImpl(Display *display, XErrorEvent *event)
	{
		LOG(WARNING) << "X error received: "
					 << "type: " << event->type << ", "
					 << "serial: " << event->serial << ", "
					 << "error_code: " << static_cast<int>(event->error_code) << ", "
					 << "request_code: " << static_cast<int>(event->request_code)
					 << ", "
					 << "minor_code: " << static_cast<int>(event->minor_code);
		return 0;
	}

	int XIOErrorHandlerImpl(Display *display)
	{
		return 0;
	}

	void displayArguments(int &argc, char *argv[], const CefRefPtr<CefCommandLine> &cefCommandLine)
    {
        std::cout<<std::endl;
        std::cout<<"---------------------------------------------------------------------------------------"<<std::endl;
        printf("ARGUMENT LIST FOR PROCESS TYPE: %d  |  ", getProcessType(cefCommandLine));
        std::cout<<"POSSIBLE PROCESS TYPES : 0 - BROWSER_PROCESS, 1 - RENDERER_PROCESS, 2 - OTHER PROCESS"<<std::endl;
        std::cout<<"ARGUMENTS: ";
        for (int i = 0; i < argc; i++)
        {
            std::cout<<" "<<argv[i];
        }
        std::cout<<std::endl;
        std::cout<<"---------------------------------------------------------------------------------------"<<std::endl;
    }

	// Entry point function for all processes.
	int main(int argc, char *argv[])
	{
	    std::cout<<config::GetStartupURL();
	    std::cout<<std::endl;

	    XSetErrorHandler(XErrorHandlerImpl);
	    XSetIOErrorHandler(XIOErrorHandlerImpl);

        // Structure for passing command-line arguments.
        // The definition of this structure is platform-specific.
        CefMainArgs mainArgs(argc, argv);

        // Optional implementation of the CefApp interface.
        CefRefPtr<CefApp> cefApplication;

        CefRefPtr<CefCommandLine> cefCommandLine = createCommandLine(mainArgs);

        switch(getProcessType(cefCommandLine))
        {
            case PROCESS_TYPE_BROWSER:
                cefApplication = createBrowserProcessApp();
                break;
            case PROCESS_TYPE_RENDERER:
                cefApplication = createRenderProcessApp();
                break;
            case PROCESS_TYPE_OTHER:
                cefApplication = createOtherProcessApp();
                break;
        }

        // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
        // that share the same executable. This function checks the command-line and,
        // if this is a sub-process, executes the appropriate logic.
        // Execute the sub-process logic, if any. This will either return immediately for the browser
        // process or block until the sub-process should exit.
        int exit_code = CefExecuteProcess(mainArgs, cefApplication.get(), nullptr);

        if (exit_code >= 0)
        {
            std::cout<<"\n process type:" << getProcessType(cefCommandLine) << " terminated with exit code: "<<exit_code<<std::endl;
            // The sub-process terminated, exit now.
            return exit_code;
        }

        displayArguments(argc, argv, cefCommandLine);

        CefSettings cefSettings;
        cefSettings.command_line_args_disabled = false;
        cefSettings.log_severity = LOGSEVERITY_INFO;
        //cefSettings.resources_dir_path = (cef_string_utf16_t) test; // shared::resourceFolderPath;
        //cefSettings.multi_threaded_message_loop = true;
        //cefSettings.remote_debugging_port = 8081;

        ClientManager clientManager;

        // Initialize CEF for the browser process. The first browser instance will be
        // created in CefBrowserProcessHandler::OnContextInitialized() after CEF has
        // been initialized.
        CefInitialize(mainArgs, cefSettings, cefApplication.get(), nullptr);

        // Run the CEF message loop. This will block until CefQuitMessageLoop() is
        // called.
        CefRunMessageLoop();

        // Shut down CEF.
        CefShutdown();

		return 0;
	}
}
