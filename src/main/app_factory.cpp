#include <include/wrapper/cef_helpers.h>
#include <include/cef_app.h>
#include <iostream>

#include "src/shared/resources_util.hpp"
#include "browser_process.hpp"
#include "render_process.hpp"

namespace main
{
    // Name of the resources folder (folder that contains html files etc)
    std::string startupLoadURL = "file://" + shared::GetProjectExecutableDir() + "resources/index.html";
}

namespace shared
{
    CefRefPtr<CefApp> createBrowserProcessApp()
    {
        return new main::CefBrowserApp(main::startupLoadURL);
    }

    CefRefPtr<CefApp> createRenderProcessApp()
    {
        return new main::CefRendererApp();
    }

    CefRefPtr<CefApp> createOtherProcessApp()
    {
        return nullptr;
    }
}