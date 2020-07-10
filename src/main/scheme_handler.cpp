#include <include/base/cef_macros.h>
#include <include/cef_base.h>
#include <include/wrapper/cef_helpers.h>
#include <include/cef_resource_handler.h>
#include <include/cef_scheme.h>
#include <iostream>

#include <src/shared/resources_util.hpp>
#include <src/shared/client_util.hpp>

#include "scheme_handler.hpp"
#include "applicationSettings.hpp"

namespace main
{
    // Implementation of the scheme handler for client:// requests.
    class ClientSchemeHandler : public CefResourceHandler
    {
    public:
        ClientSchemeHandler() : offset_(0)
        {}

        bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) override
        {
            CEF_REQUIRE_IO_THREAD();

            bool handled = false;

            std::string url = request->GetURL();

            if (strstr(url.c_str(), config::APP_SCHEME_HANDLER_FILE_NAME.c_str()) != NULL)
            {
                // Load the response html.
                if (shared::GetResourceString(config::APP_SCHEME_HANDLER_FILE_NAME, data_))
                {
                    // Insert the request contents.
                    const std::string &find = "$REQUEST$";
                    const std::string &replace = shared::DumpRequestContents(request);
                    data_.replace(data_.find(find), find.size(), replace);

                    handled = true;
                    mime_type_ = "text/html";
                }
            }
            // else if (strstr(url.c_str(), "logo.png") != NULL)
            // {
            //     // Load the response image.
            //     if (shared::GetResourceString("logo.png", data_))
            //     {
            //         handled = true;
            //         mime_type_ = "image/png";
            //     }
            // }

            if (handled)
            {
                // Indicate that the headers are available.
                callback->Continue();
                return true;
            }

            return false;
        }

        void GetResponseHeaders(CefRefPtr<CefResponse> response,
                                int64 &response_length,
                                CefString &redirectUrl) override
        {
            CEF_REQUIRE_IO_THREAD();

            DCHECK(!data_.empty());

            response->SetMimeType(mime_type_);
            response->SetStatus(200);

            // Set the resulting response length.
            response_length = data_.length();
        }

        void Cancel() OVERRIDE
        { CEF_REQUIRE_IO_THREAD(); }

        bool ReadResponse(void *data_out,
                          int bytes_to_read,
                          int &bytes_read,
                          CefRefPtr<CefCallback> callback) override
        {
            CEF_REQUIRE_IO_THREAD();

            bool has_data = false;
            bytes_read = 0;

            if (offset_ < data_.length())
            {
                // Copy the next block of data into the buffer.
                int transfer_size =
                        std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
                memcpy(data_out, data_.c_str() + offset_, transfer_size);
                offset_ += transfer_size;

                bytes_read = transfer_size;
                has_data = true;
            }

            return has_data;
        }

    private:
        std::string data_;
        std::string mime_type_;
        size_t offset_;

        IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
        DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandler);
    };

    // Implementation of the factory for creating scheme handlers.
    class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory
    {
    public:
        ClientSchemeHandlerFactory()
        {}

        // Return a new scheme handler instance to handle the request.
        CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             const CefString &scheme_name,
                                             CefRefPtr<CefRequest> request) override
        {
            CEF_REQUIRE_IO_THREAD();
            return new ClientSchemeHandler();
        }

    private:
        IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
        DISALLOW_COPY_AND_ASSIGN(ClientSchemeHandlerFactory);
    };

    void RegisterSchemeHandlerFactory()
    {
        CefRegisterSchemeHandlerFactory(config::APP_URL_SCHEME, config::APP_SCHEME_HANDLER_DOMAIN,
                                        new ClientSchemeHandlerFactory());
    }
}