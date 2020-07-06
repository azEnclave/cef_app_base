#ifndef CUSTOM_CEF_APP_APP_FACTORY_HPP
#define CUSTOM_CEF_APP_APP_FACTORY_HPP

#include <include/cef_app.h>

namespace shared
{
    // This file declares methods that must be implemented in each executable
	// target. CefApp is a global singleton that controls process-specific
	// behaviors. The returned CefApp instance will be passed to CefExecuteProcess()
	// and/or CefInitialize() by the program entry point implementation. On Linux
	// and Windows a single executable is used for all processes. On macOS a
	// separate helper executable is used for sub-processes.

	// Called in the main browser process to create the CefApp for that process.
	CefRefPtr<CefApp> createBrowserProcessApp();

	// Called in the renderer sub-process to create the CefApp for that process.
	CefRefPtr<CefApp> createRenderProcessApp();

	// Called in other sub-processes to create the CefApp for that process.
	CefRefPtr<CefApp> createOtherProcessApp();
}

#endif
