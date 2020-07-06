#include <unistd.h>
#include <string>
#include <libgen.h>

namespace shared
{
    /*
     * LINUX VERSION
     * Returns the current directory of the executable that is being run,
     * will return empty if the process CANNOT get the path via readlink() function;
     * e.g : /path/to/executableDirectory/
     *
     */
	std::string GetProjectExecutableDir()
	{
		char buffer[FILENAME_MAX];

		// Retrieve the executable path.
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 2);

		if (len == -1 || len == 0 || len >= FILENAME_MAX)
			return "";

		buffer[len] = '\0';

		// Add "_files" to the path.
		// strcpy(buffer, "_files");

		// use the dirname function to get the directory (removes the executable name from the path)
		// and add a trailing '/';
		return std::string(dirname(buffer)) + '/';
	}
}