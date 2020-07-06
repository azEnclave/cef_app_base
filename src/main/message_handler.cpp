#include "message_handler.hpp"

namespace main
{
    const char kTestMessageName[] = "some test";

    bool MessageHandler::OnQuery(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int64 query_id,
                                 const CefString &request, bool persistent,
                                 CefRefPtr<CefMessageRouterBrowserSide::Callback> callback)
    {
        // Only handle messages from the startup URL.
        const std::string &url = frame->GetURL();
        if (url.find(startupURL) != 0)
            return false;

        const std::string &message_name = request;
        if (message_name.find(kTestMessageName) == 0)
        {
            // Reverse the string and return.
            std::string result = message_name.substr(sizeof(kTestMessageName));
            std::reverse(result.begin(), result.end());
            callback->Success(result);
            return true;
        }

        return false;
    }
}