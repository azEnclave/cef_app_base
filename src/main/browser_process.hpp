#ifndef CUSTOM_CEF_APP_BROWSER_PROCESS_HPP
#define CUSTOM_CEF_APP_BROWSER_PROCESS_HPP

#include <include/cef_render_process_handler.h>
#include <include/cef_browser_process_handler.h>
#include <include/cef_scheme.h>
#include <include/cef_app.h>

namespace main
{
    class CefBrowserApp : public CefApp, CefBrowserProcessHandler
    {
    public:
        CefBrowserApp(std::string startup_URL) : startupURL(startup_URL) {};

        void OnContextInitialized() override;

        void OnBeforeCommandLineProcessing(const CefString &process_type,
                                           CefRefPtr <CefCommandLine> command_line) override;

        void OnRegisterCustomSchemes(CefRawPtr <CefSchemeRegistrar> registrar) override;

        CefRefPtr <CefBrowserProcessHandler> GetBrowserProcessHandler() override;

    private:

        std::string startupURL;

        IMPLEMENT_REFCOUNTING(CefBrowserApp);
        DISALLOW_COPY_AND_ASSIGN(CefBrowserApp);
    };
}

#endif
