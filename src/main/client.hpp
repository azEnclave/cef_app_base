#ifndef CUSTOM_CEF_APP_CLIENT_HPP
#define CUSTOM_CEF_APP_CLIENT_HPP

#include <include/cef_client.h>
#include <include/wrapper/cef_resource_manager.h>
#include <include/wrapper/cef_message_router.h>

namespace main
{
    class Client : public CefClient, public CefDisplayHandler, public CefLifeSpanHandler, public CefRequestHandler,
                   public CefResourceRequestHandler
    {
    public:
        explicit Client(const CefString &startup_url);

        // CefClient methods:
        CefRefPtr<CefDisplayHandler> GetDisplayHandler() override
        { return this; }

        CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
        { return this; }

        CefRefPtr<CefRequestHandler> GetRequestHandler() override
        { return this; }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                      CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

        // CefDisplayHandler methods:
        void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title) override;

        // CefLifeSpanHandler methods:
        void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

        bool DoClose(CefRefPtr<CefBrowser> browser) override;

        void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

        // CefRequestHandler methods:
        bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefRequest> request,
                            bool user_gesture,
                            bool is_redirect) override;

        void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) override;


        // CefResourceRequestHandler methods:
        CefRefPtr<CefResourceHandler> GetResourceHandler(
                CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                CefRefPtr<CefRequest> request) override;

        cef_return_value_t OnBeforeResourceLoad(
                CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                CefRefPtr<CefRequest> request,
                CefRefPtr<CefRequestCallback> callback) override;

        // CefRequestHandler methods:
        CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
                CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                CefRefPtr<CefRequest> request,
                bool is_navigation,
                bool is_download,
                const CefString &request_initiator,
                bool &disable_default_handling) override;

    private:
        CefRefPtr<CefResourceManager> resourceManager;

        // Handles the browser side of query routing.
        CefRefPtr<CefMessageRouterBrowserSide> messageRouter;
        scoped_ptr<CefMessageRouterBrowserSide::Handler> messageHandler;

        const std::string startupURL;

        // Track the number of browsers using this Client.
        int clientBrowserCounter;

        IMPLEMENT_REFCOUNTING(Client);
        DISALLOW_COPY_AND_ASSIGN(Client);
    };
}

#endif
