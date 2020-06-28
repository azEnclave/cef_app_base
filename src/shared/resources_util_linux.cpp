#include <unistd.h>
#include <string>

namespace shared
{
	// Returns the current directory of the executable that is being run
	std::string GetResourceDir()
	{
		char buffer[INT16_MAX];

		// Retrieve the executable path.
		ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (len == -1)
			return "";

		buffer[len] = 0;

		// Add "_files" to the path.
		// strcpy(buffer, "_files");
		return std::string(buffer);
	}
}