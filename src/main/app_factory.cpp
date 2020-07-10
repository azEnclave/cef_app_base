#include "applicationSettings.hpp"
#include "browser_process.hpp"
#include "render_process.hpp"
#include "other_process.hpp"

namespace main
{
}

namespace shared
{
    CefRefPtr<CefApp> createBrowserProcessApp()
    {
        return new main::CefBrowserApp(config::GetStartupURL());
    }

    CefRefPtr<CefApp> createRenderProcessApp()
    {
        return new main::CefRendererApp();
    }

    CefRefPtr<CefApp> createOtherProcessApp()
    {
        return new main::SubprocessApp();
    }
}