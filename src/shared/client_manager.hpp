#ifndef CUSTOM_CEF_APP_CLIENT_MANAGER_HPP
#define CUSTOM_CEF_APP_CLIENT_MANAGER_HPP

#include <list>

#include "include/base/cef_thread_checker.h"
#include "include/cef_browser.h"

namespace shared
{

// Manages multiple CefBrowser instances. All methods must be called on the
// main application thread (browser process UI thread).
    class ClientManager
    {
    public:
        ClientManager();

        ~ClientManager();

        // Returns the singleton instance of this object.
        static ClientManager *GetInstance();

        // Called from CefLifeSpanHandler methods:
        void OnAfterCreated(CefRefPtr<CefBrowser> browser);

        void DoClose(CefRefPtr<CefBrowser> browser);

        void OnBeforeClose(const CefRefPtr<CefBrowser> &browser);

        // Request that all existing browser windows close.
        void CloseAllBrowsers(bool force_close);

        // Returns true if the last browser instance is closing.
        bool IsClosing() const;

    private:
        base::ThreadChecker threadChecker;

        bool isClosing;

        // List of existing browsers.
        typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
        BrowserList browserList;
    };

}

#endif