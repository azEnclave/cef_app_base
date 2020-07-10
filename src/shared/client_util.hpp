#ifndef CUSTOM_CEF_APP_CLIENT_UTIL_HPP
#define CUSTOM_CEF_APP_CLIENT_UTIL_HPP

#include <include/cef_browser.h>

namespace shared
{
    // This file provides functionality common to all CefClient example
    // implementations.

    // Called from CefDisplayHandler methods:
    void OnTitleChange(const CefRefPtr<CefBrowser>& browser, const CefString &title);

    // Called from CefLifeSpanHandler methods:
    void OnAfterCreated(CefRefPtr<CefBrowser> browser);
    bool DoClose(CefRefPtr<CefBrowser> browser);
    void OnBeforeClose(CefRefPtr<CefBrowser> browser);

    // Platform-specific implementation.
    void PlatformTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title);

    // Returns the contents of |request| as a string.
    std::string DumpRequestContents(const CefRefPtr<CefRequest>& request);
}

#endif
