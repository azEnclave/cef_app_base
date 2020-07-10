#ifndef CUSTOM_CEF_APP_APPLICATIONSETTINGS_HPP
#define CUSTOM_CEF_APP_APPLICATIONSETTINGS_HPP

#include <src/shared/resources_util.hpp>

namespace config
{
    extern const std::string APP_URL_SCHEME;
    extern const std::string APP_SCHEME_HANDLER_DOMAIN;
    extern const std::string APP_SCHEME_HANDLER_FILE_NAME;

    extern const int  APP_SCHEME_REGISTRATION_OPTIONS;

    extern const std::string APP_PROJECT_RESOURCE_DIRECTORY_NAME;
    extern const std::string APP_PROJECT_APPLICATION_NAME;

    std::string GetStartupURL();
}

#endif
