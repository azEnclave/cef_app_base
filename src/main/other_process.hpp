#ifndef CUSTOM_CEF_APP_OTHER_PROCESS_HPP
#define CUSTOM_CEF_APP_OTHER_PROCESS_HPP

#include <include/cef_scheme.h>
#include <include/cef_app.h>

namespace main
{
    // Implementation of CefApp for all subprocesses.
    class SubprocessApp : public CefApp
    {
    public:
        SubprocessApp() {}

        // CefApp methods:
        void OnRegisterCustomSchemes(CefRawPtr <CefSchemeRegistrar> registrar) override;
    private:
        IMPLEMENT_REFCOUNTING(SubprocessApp);
        DISALLOW_COPY_AND_ASSIGN(SubprocessApp);
    };
}
#endif
