#include "fileHandler.h"

#include <fstream>
#include <sstream>

using namespace std;

string readFile(const string &filePath)
{

    ifstream file(filePath);

    if (!file.is_open())
    {
        return "";
    }

    stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}

string getMimeType(const string &path)
{

    if (path.find(".html") != string::npos)
    {
        return "text/html";
    }

    if (path.find(".css") != string::npos)
    {
        return "text/css";
    }

    if (path.find(".js") != string::npos)
    {
        return "application/javascript";
    }

    return "text/plain";
}