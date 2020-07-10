#ifndef CUSTOM_CEF_APP_RENDER_PROCESS_HPP
#define CUSTOM_CEF_APP_RENDER_PROCESS_HPP

#include <include/cef_render_process_handler.h>
#include <include/wrapper/cef_message_router.h>
#include <include/cef_app.h>

namespace main
{
    // Implementation of CefApp for the renderer process.
    class CefRendererApp : public CefApp, CefRenderProcessHandler
    {
    public:
        CefRendererApp()
        {}

        // CefApp methods:
        CefRefPtr <CefRenderProcessHandler> GetRenderProcessHandler() override;

        void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

        // CefRenderProcessHandler methods:
        void OnWebKitInitialized() override;

        void OnContextCreated(CefRefPtr <CefBrowser> browser,
                              CefRefPtr <CefFrame> frame,
                              CefRefPtr <CefV8Context> context) override;

        void OnContextReleased(CefRefPtr <CefBrowser> browser,
                               CefRefPtr <CefFrame> frame,
                               CefRefPtr <CefV8Context> context) override;

        bool OnProcessMessageReceived(CefRefPtr <CefBrowser> browser,
                                      CefRefPtr <CefFrame> frame,
                                      CefProcessId source_process,
                                      CefRefPtr <CefProcessMessage> message) override;

    private:
        // Handles the renderer side of query routing.
        CefRefPtr <CefMessageRouterRendererSide> messageRouter;

        IMPLEMENT_REFCOUNTING(CefRendererApp);
        DISALLOW_COPY_AND_ASSIGN(CefRendererApp);
    };
}

#endif