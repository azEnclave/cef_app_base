#ifndef CUSTOM_CEF_APP_WINDOW_DELEGATE_HPP
#define CUSTOM_CEF_APP_WINDOW_DELEGATE_HPP

#include <include/views/cef_window_delegate.h>
#include <include/views/cef_browser_view.h>

namespace shared
{
    // When using the Views framework this object provides the delegate
    // implementation for the CefWindow that hosts the Views-based browser.
    class WindowDelegate : public CefWindowDelegate
    {
    public:
        explicit WindowDelegate(const CefRefPtr<CefBrowserView> &browser_view) : browserView(browser_view) {};

        void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
        void OnWindowCreated(CefRefPtr<CefWindow> window) override;
        bool CanClose(CefRefPtr<CefWindow> window) override;
        CefSize GetPreferredSize(CefRefPtr<CefView> view) override;
        CefSize GetMinimumSize(CefRefPtr<CefView> view) override;

    private:
        CefRefPtr<CefBrowserView> browserView;
        IMPLEMENT_REFCOUNTING(WindowDelegate);
        DISALLOW_COPY_AND_ASSIGN(WindowDelegate);
    };

}

#endif
