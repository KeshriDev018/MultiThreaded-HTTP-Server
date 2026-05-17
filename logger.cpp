#include "logger.h"
#include <ctime>
#include <iomanip>
#include <fstream>
#include <iostream>

using namespace std;

void logRequest(const string &method,
                const string &path,
                const string &clientIP,
                int statusCode,
                const string &statusText)
{

    ofstream logFile(
        "logs/server.log",
        ios::app);

    if (logFile.is_open())
    {

        time_t now = time(0);

        tm *localTime = localtime(&now);

        logFile
            << "["
            << put_time(localTime, "%Y-%m-%d %H:%M:%S")
            << "] "
            << clientIP
            << " "
            << method
            << " "
            << path
            << " "
            << statusCode
            << " "
            << statusText
            << endl;
    }
}