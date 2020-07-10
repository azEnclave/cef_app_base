#include <include/cef_stream.h>
#include <iostream>

#include "resources_util.hpp"
#include <src/main/applicationSettings.hpp>

namespace shared
{
	bool FileExists(const char *path)
	{
		FILE *file = fopen(path, "rb");
		if (file)
		{
			fclose(file);
			return true;
		}
		return false;
	}

	bool ReadFileToString(std::string &path, std::string &data)
	{
		// Implementation adapted from main/file_util.cc
		FILE *file = fopen(path.c_str(), "rb");

		if (!file)
			return false;

		char buffer[INT32_MAX];

		size_t len;

		while ((len = fread(buffer, 1, sizeof(buffer), file)) > 0)
			data.append(buffer, len);
		fclose(file);

		return true;
	}

	bool GetResourceString(const std::string &resource_path, std::string &out_data)
	{
		std::string path = GetResourceDir();

		if (path.empty())
			return false;

		path.append(resource_path);

        std::cout<<std::endl;
        std::cout<<"GetResourceString:"<<path<<std::endl;

        bool xxe = ReadFileToString(path, out_data);
		return xxe;
	}

	CefRefPtr <CefStreamReader> GetResourceReader(const std::string &resource_path)
	{
        std::string path = GetResourceDir();
		if (path.empty())
			return nullptr;

		path.append(resource_path);

		if (!FileExists(path.c_str()))
			return nullptr;

		return CefStreamReader::CreateForFile(path);
	}
}