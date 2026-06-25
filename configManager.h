#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

using namespace std;

class ConfigManager
{
public:
    
    static bool loadConfig();

    
    static int getPort();

private:
    static int port;
    static bool isLoaded;
};

#endif
