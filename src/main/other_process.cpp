#include "other_process.hpp"
#include "applicationSettings.hpp"

namespace main
{
    void SubprocessApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
    {
        // Register the custom scheme as standard and secure.
        // Must be the same implementation in all processes.
        registrar->AddCustomScheme(config::APP_URL_SCHEME, config::APP_SCHEME_REGISTRATION_OPTIONS);
    }
}