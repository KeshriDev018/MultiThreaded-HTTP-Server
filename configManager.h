#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

using namespace std;

/**
 * Simple configuration file reader.
 * Reads config.txt for server settings.
 * Format: KEY=VALUE
 * Example: PORT=8080
 */
class ConfigManager
{
public:
    /**
     * Load configuration from config.txt
     * @return true if config loaded successfully, false otherwise
     */
    static bool loadConfig();

    /**
     * Get server port from configuration
     * @return Port number (default 8080 if not configured)
     */
    static int getPort();

private:
    static int port;
    static bool isLoaded;
};

#endif
