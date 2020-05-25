#include <include/cef_app.h>

namespace shared
{
	// No CefApp for the renderer sub-process.
	CefRefPtr<CefApp> CreateRendererProcessApp()
	{
		return NULL;
	}
}

