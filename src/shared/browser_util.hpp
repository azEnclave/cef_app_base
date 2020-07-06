#ifndef CUSTOM_CEF_APP_BROWSER_UTIL_HPP
#define CUSTOM_CEF_APP_BROWSER_UTIL_HPP

#include <include/internal/cef_string.h>
#include <include/internal/cef_types_wrappers.h>
#include <include/cef_client.h>

namespace  shared
{
    // Helper for creating a new CefBrowser instance. Usually called from the
    // CefBrowserProcessHandler::OnContextInitialized method. Respects the
    // "--use-views" command-line flag if specified. Must be called on the browser
    // process UI thread.
    void CreateBrowser(const CefRefPtr<CefClient> &client, const CefString &startup_url, const CefBrowserSettings &settings);
}

#endif
