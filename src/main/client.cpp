#include <src/shared/resources_util.hpp>
#include <src/shared/client_util.hpp>
#include <iostream>

#include "client.hpp"
#include "message_handler.hpp"

namespace main
{
    Client::Client(const CefString &startup_url) : startupURL(startup_url), clientBrowserCounter(0)
    {}

    void Client::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title)
    {
        // Call the default shared implementation.
        shared::OnTitleChange(browser, title);
    }

    bool Client::DoClose(CefRefPtr<CefBrowser> browser)
    {
        // Call the default shared implementation.
        return shared::DoClose(browser);
    }

    void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        if (!messageRouter)
        {
            // Create the browser-side router for query handling.
            CefMessageRouterConfig config;
            messageRouter = CefMessageRouterBrowserSide::Create(config);

            // Register handlers with the router.
            messageHandler.reset(new MessageHandler(startupURL));
            messageRouter->AddHandler(messageHandler.get(), false);
        }

        clientBrowserCounter++;

        // Call the default shared implementation.
        shared::OnAfterCreated(browser);
    }

    void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        if (--clientBrowserCounter == 0)
        {
            // Free the router when the last browser is closed.
            messageRouter->RemoveHandler(messageHandler.get());
            messageHandler.reset();
            messageRouter = nullptr;
        }

        shared::OnBeforeClose(browser);
    }

    bool Client::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request,
                                bool user_gesture, bool is_redirect)
    {
        return CefRequestHandler::OnBeforeBrowse(browser, frame, request, user_gesture, is_redirect);
    }

    CefRefPtr<CefResourceRequestHandler>
    Client::GetResourceRequestHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefRequest> request, bool is_navigation, bool is_download,
                                      const CefString &request_initiator, bool &disable_default_handling)
    {
        CEF_REQUIRE_IO_THREAD();
        return this;
    }

    void Client::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, CefRequestHandler::TerminationStatus status)
    {
        CEF_REQUIRE_UI_THREAD();
        messageRouter->OnRenderProcessTerminated(browser);
    }

    CefRefPtr<CefResourceHandler>
    Client::GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request)
    {
        CEF_REQUIRE_IO_THREAD();
        const std::string& url = request->GetURL();
        // This is a minimal implementation of resource loading. For more complex
        // usage (multiple files, zip archives, custom handlers, etc.) you might want
        // to use CefResourceManager. See the "resource_manager" target for an
        // example implementation.
        const std::string& resource_path = shared::GetResourcePath(url);

        if (!resource_path.empty())
            return shared::GetResourceHandler(resource_path);

        return nullptr;
    }

    bool Client::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        CEF_REQUIRE_UI_THREAD();
        return messageRouter->OnProcessMessageReceived(browser, frame, source_process, message);
    }
}
