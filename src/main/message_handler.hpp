#ifndef CUSTOM_CEF_APP_MESSAGE_HANDLER_HPP
#define CUSTOM_CEF_APP_MESSAGE_HANDLER_HPP

#include <include/base/cef_macros.h>
#include <include/internal/cef_string.h>
#include <include/wrapper/cef_message_router.h>

namespace main
{

    // Handle messages in the browser process.
    class MessageHandler : public CefMessageRouterBrowserSide::Handler
    {
    public:
        explicit MessageHandler(const CefString &startup_url) : startupURL(startup_url) {}

        // Called due to cefQuery execution in message_router.html.
        bool OnQuery(CefRefPtr <CefBrowser> browser,
                     CefRefPtr <CefFrame> frame,
                     int64 query_id,
                     const CefString &request,
                     bool persistent,
                     CefRefPtr <Callback> callback);

    private:
        const CefString startupURL;

        DISALLOW_COPY_AND_ASSIGN(MessageHandler);
    };
}

#endif
