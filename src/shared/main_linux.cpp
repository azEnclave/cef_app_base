#include "main.hpp"

#include <X11/Xlib.h>

#include <include/cef_command_line.h>
#include <include/cef_app.h>
#include <iostream>

#include "app_factory.hpp"
#include "main_util.hpp"
#include "client_manager.hpp"
#include "resources_util.hpp"

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

	// Entry point function for all processes.
	int main(int argc, char *argv[])
	{
		std::string resource_dir;
		if(shared::GetResourceDir(resource_dir))
		{
			std::cout<<resource_dir;
		}


		// Install Xlib error handlers so that the application won't be terminated
		// on non-fatal errors.
		XSetErrorHandler(XErrorHandlerImpl);
		XSetIOErrorHandler(XIOErrorHandlerImpl);

		// Provide CEF with command-line arguments.
		CefMainArgs main_args(argc, argv);

		// Create a temporary CommandLine object.
		CefRefPtr <CefCommandLine> command_line = CreateCommandLine(main_args);

		// Create a CefApp of the correct process type.
		CefRefPtr <CefApp> app;
		switch (GetProcessType(command_line))
		{
			case PROCESS_TYPE_BROWSER:
				app = shared::CreateBrowserProcessApp();
				break;
			case PROCESS_TYPE_RENDERER:
				app = shared::CreateRendererProcessApp();
				break;
			case PROCESS_TYPE_OTHER:
				app = shared::CreateOtherProcessApp();
				break;
		}

		std::cout<<std::endl;
		printf("ARGUMENT LIST FOR PROCESS TYPE: %d  |  ", GetProcessType(command_line));
		std::cout<<"POSSIBLE PROCESS TYPES : 0 - BROWSER_PROCESS, 1 - RENDERER_PROCESS, 2 - OTHER PROCESS"<<std::endl;
		std::cout<<"ARGUMENTS: ";
		for (unsigned long i = 0; i < sizeof(&argv)/sizeof(char); i++)
		{
			std::cout<<" "<<argv[i];
		}
		std::cout<<std::endl;

		// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
		// that share the same executable. This function checks the command-line and,
		// if this is a sub-process, executes the appropriate logic.
		int exit_code = CefExecuteProcess(main_args, app, NULL);

		if (exit_code >= 0)
		{
			// The sub-process has completed so return here.
			return exit_code;
		}

		// Create the singleton manager instance.
		ClientManager manager;

		// Specify CEF global settings here.
		CefSettings settings;
		settings.command_line_args_disabled = false;
		settings.log_severity = LOGSEVERITY_DEFAULT;

		// Initialize CEF for the browser process. The first browser instance will be
		// created in CefBrowserProcessHandler::OnContextInitialized() after CEF has
		// been initialized.
		CefInitialize(main_args, settings, app, NULL);

		// Run the CEF message loop. This will block until CefQuitMessageLoop() is
		// called.
		CefRunMessageLoop();

		// Shut down CEF.
		CefShutdown();

		return 0;
	}
}
