#ifndef CUSTOM_CEF_APP_CLIENT_HPP
#define CUSTOM_CEF_APP_CLIENT_HPP

#include "include/cef_client.h"

namespace base
{
	// Minimal implementation of client handlers.
	class Client : public CefClient,
				   public CefDisplayHandler,
				   public CefLifeSpanHandler
	{
	public:
		Client();

		// CefClient methods:
		CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE
		{ return this; }

		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
		{ return this; }

		// CefDisplayHandler methods:
		void OnTitleChange(CefRefPtr<CefBrowser> browser,
						   const CefString &title) OVERRIDE;

		// CefLifeSpanHandler methods:
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

		bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	private:
		IMPLEMENT_REFCOUNTING(Client);
		DISALLOW_COPY_AND_ASSIGN(Client);
	};
}
#endif
