#include "src/shared/browser_util.hpp"
#include "src/shared/app_factory.hpp"
#include "client.hpp"

namespace base
{
	const char kStartupURL[] = "https://www.google.com";

	// Minimal implementation of CefApp for the browser process.
	class BrowserApp : public CefApp, public CefBrowserProcessHandler
	{
	public:
		BrowserApp()
		{}

		// CefApp methods:
		CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE
		{
			return this;
		}

		void OnBeforeCommandLineProcessing(
				const CefString &process_type,
				CefRefPtr<CefCommandLine> command_line) OVERRIDE
		{
			// Command-line flags can be modified in this callback.
			// |process_type| is empty for the browser process.
			if (process_type.empty())
			{
#if defined(OS_MACOSX)
				// Disable the macOS keychain prompt. Cookies will not be encrypted.
		  command_line->AppendSwitch("use-mock-keychain");
#endif
			}
		}

		// CefBrowserProcessHandler methods:
		void OnContextInitialized() OVERRIDE
		{
			// Create the browser window.
			shared::CreateBrowser(new Client(), kStartupURL, CefBrowserSettings());
		}

	private:
		IMPLEMENT_REFCOUNTING(BrowserApp);
		DISALLOW_COPY_AND_ASSIGN(BrowserApp);
	};

}

namespace shared
{
	CefRefPtr<CefApp> CreateBrowserProcessApp()
	{
		return new base::BrowserApp();
	}
}