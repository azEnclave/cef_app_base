#include "client.hpp"
#include "src/shared/client_util.hpp"

namespace base
{
	Client::Client()
	{}

	void Client::OnTitleChange(CefRefPtr <CefBrowser> browser,
							   const CefString &title)
	{
		// Call the default shared implementation.
		shared::OnTitleChange(browser, title);
	}

	void Client::OnAfterCreated(CefRefPtr <CefBrowser> browser)
	{
		// Call the default shared implementation.
		shared::OnAfterCreated(browser);
	}

	bool Client::DoClose(CefRefPtr <CefBrowser> browser)
	{
		// Call the default shared implementation.
		return shared::DoClose(browser);
	}

	void Client::OnBeforeClose(CefRefPtr <CefBrowser> browser)
	{
		// Call the default shared implementation.
		return shared::OnBeforeClose(browser);
	}
}