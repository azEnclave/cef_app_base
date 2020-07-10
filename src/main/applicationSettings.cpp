#include "applicationSettings.hpp"

namespace config
{
    extern const std::string APP_URL_SCHEME = "client";
    extern const std::string APP_SCHEME_HANDLER_DOMAIN = "local";
    extern const std::string APP_SCHEME_HANDLER_FILE_NAME = "index.html";

    extern const int  APP_SCHEME_REGISTRATION_OPTIONS = CEF_SCHEME_OPTION_STANDARD | CEF_SCHEME_OPTION_SECURE | CEF_SCHEME_OPTION_CORS_ENABLED | CEF_SCHEME_OPTION_FETCH_ENABLED;

    extern const std::string APP_PROJECT_RESOURCE_DIRECTORY_NAME = "resources";
    extern const std::string APP_PROJECT_APPLICATION_NAME = "cef_custom_app";

    std::string GetStartupURL()
    {
        std::stringstream ss;
        ss << std::string(APP_URL_SCHEME)  << "://" << APP_SCHEME_HANDLER_DOMAIN << "/" <<
        // shared::GetResourceDir(APP_PROJECT_RESOURCE_DIRECTORY_NAME) <<
        "" << APP_SCHEME_HANDLER_FILE_NAME;
        return ss.str();
    }
}