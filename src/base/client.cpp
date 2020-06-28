#include "src/shared/resources_util.hpp"
#include "client.hpp"
#include "src/shared/client_util.hpp"
#include <sstream>

#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_stream_resource_handler.h"

namespace base
{
	// Demonstrate a custom Provider implementation by dumping the request contents.
	class RequestDumpResourceProvider : public CefResourceManager::Provider
	{
	public:
		explicit RequestDumpResourceProvider(const std::string &url) : url_(url)
		{
			DCHECK(!url.empty());
		}

		bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE
		{
			CEF_REQUIRE_IO_THREAD();

			const std::string &url = request->url();
			if (url != url_)
			{
				// Not handled by this provider.
				return false;
			}

			const std::string &dump = shared::DumpRequestContents(request->request());
			std::string str =
					"<html><body bgcolor=\"white\"><pre>" + dump + "</pre></body></html>";
			CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(
					static_cast<void *>(const_cast<char *>(str.c_str())), str.size());
			DCHECK(stream.get());
			request->Continue(new CefStreamResourceHandler("text/html", stream));
			return true;
		}

	private:
		std::string url_;

		DISALLOW_COPY_AND_ASSIGN(RequestDumpResourceProvider);
	};

	void SetupResourceManager(const CefRefPtr<CefResourceManager>& _resourceManager)
	{
		if (!CefCurrentlyOn(TID_IO))
		{
			// Execute on the browser IO thread.
			CefPostTask(TID_IO, base::Bind(SetupResourceManager, _resourceManager));
			return;
		}

		const std::string &test_origin = shared::rootDir;

		// Add the Provider for dumping request contents.
		_resourceManager->AddProvider(
				new RequestDumpResourceProvider(test_origin + "request.html"), 0,
				std::string());

		// Add the Provider for bundled resource files.
#if defined(OS_WIN)
		// Read BINARY base_files from the executable.
		resource_manager->AddProvider(shared::CreateBinaryResourceProvider(test_origin), 100, std::string());
#elif defined(OS_POSIX)
		// Read individual resource files from a directory on disk.
		std::string resource_dir = shared::GetResourceDir();
		if (!resource_dir.empty())
		{
			_resourceManager->AddDirectoryProvider(test_origin, resource_dir, 100,std::string());
		}
#endif
	}

	Client::Client()
	{
		resourceManager = new CefResourceManager();
		SetupResourceManager(resourceManager);
	}

	void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,
							   const CefString &title)
	{
		// Call the default shared implementation.
		shared::OnTitleChange(browser, title);
	}

	void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser)
	{
		// Call the default shared implementation.
		shared::OnAfterCreated(browser);
	}

	bool Client::DoClose(CefRefPtr<CefBrowser> browser)
	{
		// Call the default shared implementation.
		return shared::DoClose(browser);
	}

	void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser)
	{
		// Call the default shared implementation.
		return shared::OnBeforeClose(browser);
	}

	CefRefPtr<CefResourceRequestHandler> Client::GetResourceRequestHandler(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			bool is_navigation,
			bool is_download,
			const CefString &request_initiator,
			bool &disable_default_handling)
	{
		CEF_REQUIRE_IO_THREAD();
		return this;
	}

	cef_return_value_t Client::OnBeforeResourceLoad(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			CefRefPtr<CefRequestCallback> callback)
	{
		CEF_REQUIRE_IO_THREAD();

		return resourceManager->OnBeforeResourceLoad(browser, frame, request, callback);
	}

	CefRefPtr<CefResourceHandler> Client::GetResourceHandler(
			CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request)
	{
		CEF_REQUIRE_IO_THREAD();

		return resourceManager->GetResourceHandler(browser, frame, request);
	}
}