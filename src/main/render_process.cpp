#include "render_process.hpp"

namespace main
{
    CefRefPtr<CefRenderProcessHandler> CefRendererApp::GetRenderProcessHandler()
    {
        return this;
    }

    // CefRenderProcessHandler methods:
    void CefRendererApp::OnWebKitInitialized()
    {
        // Create the renderer-side router for query handling.
        CefMessageRouterConfig config;
        messageRouter = CefMessageRouterRendererSide::Create(config);
    }

    void CefRendererApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context)
    {
        messageRouter->OnContextCreated(browser, frame, context);
    }

    void CefRendererApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefV8Context> context)
    {
        messageRouter->OnContextReleased(browser, frame, context);
    }

    bool CefRendererApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                               CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        return messageRouter->OnProcessMessageReceived(browser, frame, source_process, message);
    }
}