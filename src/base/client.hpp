#ifndef CUSTOM_CEF_APP_CLIENT_HPP
#define CUSTOM_CEF_APP_CLIENT_HPP

#include <include/wrapper/cef_resource_manager.h>
#include <include/wrapper/cef_stream_resource_handler.h>
#include <src/shared/client_util.hpp>
#include "include/cef_client.h"

namespace base
{
	// Implementation of client handlers.
	class Client : public CefClient,
				   public CefDisplayHandler,
				   public CefLifeSpanHandler,
				   public CefRequestHandler,
				   public CefResourceRequestHandler
	{
	public:
		Client();

		// CefClient methods:
		CefRefPtr<CefDisplayHandler> GetDisplayHandler() override
		{ return this; }

		CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
		{ return this; }

		CefRefPtr<CefRequestHandler> GetRequestHandler() override
		{ return this; }

		// CefDisplayHandler methods:
		void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString &title) override;

		// CefLifeSpanHandler methods:
		void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;

		bool DoClose(CefRefPtr<CefBrowser> browser) override;

		void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

		// CefRequestHandler methods:
		CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefRequest> request,
				bool is_navigation,
				bool is_download,
				const CefString &request_initiator,
				bool &disable_default_handling) override;

		// CefResourceRequestHandler methods:
		cef_return_value_t OnBeforeResourceLoad(
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefRequest> request,
				CefRefPtr<CefRequestCallback> callback) override;

		CefRefPtr<CefResourceHandler> GetResourceHandler(
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefRequest> request) override;

	private:
		// Manages the registration and delivery of base_files.
		CefRefPtr<CefResourceManager> resourceManager;
	IMPLEMENT_REFCOUNTING(Client);
		DISALLOW_COPY_AND_ASSIGN(Client);
	};
}

#endif
