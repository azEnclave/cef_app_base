#include "window_delegate.hpp"

#include "include/cef_command_line.h"
#include "include/views/cef_window.h"

namespace shared
{
    void WindowDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window)
    {
        browserView = nullptr;
    }

    void WindowDelegate::OnWindowCreated(CefRefPtr<CefWindow> window)
    {
        // Add the browser view and show the window.
        window->AddChildView(browserView);
        window->Show();

        // Give keyboard focus to the browser view.
        browserView->RequestFocus();
    }

    bool WindowDelegate::CanClose(CefRefPtr<CefWindow> window)
    {
        // Allow the window to close if the browser says it's OK.
        CefRefPtr<CefBrowser> browser = browserView->GetBrowser();
        if (browser)
            return browser->GetHost()->TryCloseBrowser();
        return true;
    }

    CefSize WindowDelegate::GetPreferredSize(CefRefPtr<CefView> view)
    {
        return CefSize(1280, 720);
    }

    CefSize WindowDelegate::GetMinimumSize(CefRefPtr<CefView> view)
    {
        return CefSize(480, 320);
    }
}