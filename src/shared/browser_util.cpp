#include <include/wrapper/cef_helpers.h>
#include <include/views/cef_browser_view.h>
#include <include/views/cef_window.h>
#include <include/cef_command_line.h>

#include "browser_util.hpp"
#include "window_delegate.hpp"

namespace shared
{
    void CreateBrowser(const CefRefPtr<CefClient> &client, const CefString &startup_url, const CefBrowserSettings &settings)
    {
        CEF_REQUIRE_UI_THREAD();

#if defined(OS_WIN) || defined(OS_LINUX)
        CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();

        // Create the browser using the Views framework if "--use-views" is specified
        // via the command-line. Otherwise, create the browser using the native
        // platform framework. The Views framework is currently only supported on
        // Windows and Linux.
        const bool use_views = command_line->HasSwitch("use-views");
#else
        const bool use_views = false;
#endif

        if (use_views)
        {
            // Create the BrowserView.
            CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
                    client, startup_url, settings, NULL, NULL, NULL);

            // Create the Window. It will show itself after creation.
            CefWindow::CreateTopLevelWindow(new WindowDelegate(browser_view));
        }
        else
        {
            // Information used when creating the native window.
            CefWindowInfo window_info;

#if defined(OS_WIN)
            // On Windows we need to specify certain flags that will be passed to
            // CreateWindowEx().
            window_info.SetAsPopup(NULL, "main");
#endif

            // Create the browser window.
            CefBrowserHost::CreateBrowser(window_info, client, startup_url, settings,
                                          NULL, NULL);
        }
    }
}