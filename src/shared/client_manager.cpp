#include <include/cef_app.h>
#include <include/wrapper/cef_helpers.h>
#include "client_manager.hpp"

namespace shared
{
    ClientManager* g_manager = NULL;

    ClientManager::ClientManager() : isClosing(false)
    {
        g_manager = this;
    }

    ClientManager::~ClientManager()
    {
        DCHECK(threadChecker.CalledOnValidThread());
        DCHECK(browserList.empty());
        g_manager = NULL;
    }

    ClientManager *ClientManager::GetInstance()
    {
        CEF_REQUIRE_UI_THREAD();
        DCHECK(g_manager);
        return g_manager;
    }

    void ClientManager::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        DCHECK(threadChecker.CalledOnValidThread());

        // Add to the list of existing browsers.
        browserList.push_back(browser);
    }

    void ClientManager::DoClose(CefRefPtr<CefBrowser> browser)
    {
        DCHECK(threadChecker.CalledOnValidThread());

        if (browserList.size() == 1U)
        {
            // The last browser window is closing.
            isClosing = true;
        }
    }

    void ClientManager::OnBeforeClose(const CefRefPtr<CefBrowser> &browser)
    {
        // Remove from the list of existing browsers.
        BrowserList::iterator bit = browserList.begin();
        for (; bit != browserList.end(); ++bit)
        {
            if ((*bit)->IsSame(browser))
            {
                browserList.erase(bit);
                break;
            }
        }

        if (browserList.empty())
        {
            // All browser windows have closed. Quit the application message loop.
            CefQuitMessageLoop();
        }
    }

    void ClientManager::CloseAllBrowsers(bool force_close)
    {
        DCHECK(threadChecker.CalledOnValidThread());

        if (!browserList.empty())
        {
            return;
        }

        BrowserList::const_iterator it = browserList.begin();
        for (; it != browserList.end(); it++)
        {
            (*it)->GetHost()->CloseBrowser(force_close);
        }
    }

    bool ClientManager::IsClosing() const
    {
        DCHECK(threadChecker.CalledOnValidThread());
        return isClosing;
    }
}
