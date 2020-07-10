#include <iostream>
#include <include/wrapper/cef_stream_resource_handler.h>

#include "client.hpp"
#include "message_handler.hpp"
#include "applicationSettings.hpp"
#include <src/shared/resources_util.hpp>
#include <src/shared/client_util.hpp>

namespace main
{
    // Demonstrate a custom Provider implementation by dumping the request contents.
    class RequestDumpResourceProvider : public CefResourceManager::Provider
    {
    public:
        explicit RequestDumpResourceProvider(const std::string &url) : url_(url)
        {
            DCHECK(!url.empty());
        }

        bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE
        {
            CEF_REQUIRE_IO_THREAD();

            const std::string &url = request->url();
            if (url != url_)
            {
                // Not handled by this provider.
                return false;
            }

            const std::string &dump = shared::DumpRequestContents(request->request());
            std::string str = "<html><body bgcolor=\"white\"><pre>" + dump + "</pre></body></html>";
            CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(
                    static_cast<void *>(const_cast<char *>(str.c_str())), str.size());

            DCHECK(stream.get());

            request->Continue(new CefStreamResourceHandler("text/html", stream));
            return true;
        }

    private:
        std::string url_;

        DISALLOW_COPY_AND_ASSIGN(RequestDumpResourceProvider);
    };

    // Add example Providers to the CefResourceManager.
    void SetupResourceManager(const CefRefPtr<CefResourceManager>& resource_manager)
    {
        if (!CefCurrentlyOn(TID_IO))
        {
            // Execute on the browser IO thread.
            CefPostTask(TID_IO, base::Bind(SetupResourceManager, resource_manager));
            return;
        }

        // Add the Provider for dumping request contents.
        resource_manager->AddProvider(
                new RequestDumpResourceProvider(shared::GetProjectExecutableDir() + "request.html"), 0,
                std::string());

// Add the Provider for bundled resource files.
#if defined(OS_WIN)
        // Read BINARY resources from the executable.
        resource_manager->AddProvider(
        shared::CreateBinaryResourceProvider(test_origin), 100, std::string());
#elif defined(OS_POSIX)
        // Read individual resource files from a directory on disk.
        std::string resource_dir = shared::GetResourceDir();

        if (!resource_dir.empty())
        {
            resource_manager->AddDirectoryProvider(config::GetStartupURL(), resource_dir, 100,
                                                   std::string());
        }
#endif
    }


    Client::Client(const CefString &startup_url) : startupURL(startup_url), clientBrowserCounter(0)
    {
        resourceManager = new CefResourceManager();
        SetupResourceManager(resourceManager);
    }

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
        return resourceManager->GetResourceHandler(browser, frame, request);
    }

    bool Client::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
    {
        CEF_REQUIRE_UI_THREAD();
        return messageRouter->OnProcessMessageReceived(browser, frame, source_process, message);
    }

    cef_return_value_t
    Client::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback)
    {
        CEF_REQUIRE_IO_THREAD();
        return resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
    }
}
