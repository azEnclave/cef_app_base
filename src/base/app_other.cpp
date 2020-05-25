#include <include/cef_app.h>

namespace shared
{
	// No CefApp for other sub-processes.
	CefRefPtr <CefApp> CreateOtherProcessApp()
	{
		return NULL;
	}
}
