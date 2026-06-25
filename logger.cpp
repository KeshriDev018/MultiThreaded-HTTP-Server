#include "logger.h"
#include <ctime> //This library provides:time() ,localtime()
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

    //ofstream is used to write file.its is a class we create a object logfile and ios is a class inside it there are constants one of them is 
    //app which means append i.e while writing new logs append it with old dont erase old logs 

    ofstream logFile(
        "logs/server.log",
        ios::app);

    if (logFile.is_open())
    {

        time_t now = time(0); //time_t :datatype to store timestamps

        tm *localTime = localtime(&now); //tm is a structure of {year,month,day,hour,min,second} localtime converts the now timestamp in this structure

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

// Its entire job is: Write request information into a log file