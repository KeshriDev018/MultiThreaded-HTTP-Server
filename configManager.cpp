#include "configManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Static member initialization
int ConfigManager::port = 8080; // Default port
bool ConfigManager::isLoaded = false;

bool ConfigManager::loadConfig()
{
    if (isLoaded)
    {
        return true; // Already loaded
    }

    ifstream configFile("config.txt");
    // Open config.txt for reading.


    // If config.txt doesn't exist, use defaults
    if (!configFile.is_open())
    {
        cout << "[CONFIG] config.txt not found. Using default port: " << port << endl;
        return true; // Not an error, just use defaults
    }

    // getline() is used to read an entire line of text from an input stream.
    //Syntax
    // getline(inputStream, stringVariable);
    
    string line;
    while (getline(configFile, line)) // Read File Line By Line
    {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        
        //size_t = unsigned int,cannot be negative
        //npos = not found
        size_t delimPos = line.find('=');
        if (delimPos == string::npos)
        {
            continue; // Invalid line
        }

        string key = line.substr(0, delimPos);
        string value = line.substr(delimPos + 1);

        // Trim whitespace from key and value
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        // Parse PORT configuration
        if (key == "PORT")
        {
            try
            {
                int configPort = stoi(value);
                if (configPort > 0 && configPort < 65536)
                {
                    port = configPort;
                    cout << "[CONFIG] Loaded PORT=" << port << " from config.txt" << endl;
                }
                else
                {
                    cout << "[CONFIG] Invalid port number: " << configPort << ". Using default: " << port << endl;
                }
            }
            catch (const exception &e)
            {
                cout << "[CONFIG] Failed to parse PORT value: " << value << ". Using default: " << port << endl;
            }
        }
    }

    configFile.close();
    isLoaded = true;
    return true;
}

int ConfigManager::getPort()
{
    if (!isLoaded)
    {
        loadConfig();
    }
    return port;
}

// ConfigManager loads the port from config.txt.If the file is missing, invalid, or the port is invalid, it falls back to the default port(8080).Then getPort() returns the final port value to the server.

//  ifstream stands for:Input File Stream
// It is used to read data from a file.
// Syntax :  ifstream file("config.txt");  or
//  ifstream file;
// file.open("config.txt");

