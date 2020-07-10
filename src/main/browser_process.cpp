#include "applicationSettings.hpp"
#include "browser_process.hpp"
#include "client.hpp"
#include "scheme_handler.hpp"
#include "src/shared/browser_util.hpp"

namespace main
{
    void CefBrowserApp::OnContextInitialized()
    {
        // Register the custom scheme handler factory.
        RegisterSchemeHandlerFactory();

        // Create the browser window.
        shared::CreateBrowser(new Client(startupURL), startupURL, CefBrowserSettings());
    }

    void CefBrowserApp::OnBeforeCommandLineProcessing(const CefString &process_type, CefRefPtr<CefCommandLine> command_line)
    {
        // Command-line flags can be modified in this callback.
        // |process_type| is empty for the browser process.
        if (process_type.empty())
        {
#if defined(OS_MACOSX)
            // Disable the macOS keychain prompt. Cookies will not be encrypted.
            command_line->AppendSwitch("use-mock-keychain");
#endif
        }
    }

    void CefBrowserApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
    {
        // Register the custom scheme as standard and secure.
        // Must be the same implementation in all processes.
        registrar->AddCustomScheme(config::APP_URL_SCHEME, config::APP_SCHEME_REGISTRATION_OPTIONS);
    }

    CefRefPtr<CefBrowserProcessHandler> CefBrowserApp::GetBrowserProcessHandler()
    {
        return this;
    }
}