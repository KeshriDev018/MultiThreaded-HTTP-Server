#include "fileHandler.h"

#include <fstream>
#include <sstream>

using namespace std;


//Input: File path
// Output: File contents

string readFile(const string &filePath)
{

    ifstream file(filePath);

    if (!file.is_open())
    {
        return "";
    }

    stringstream buffer;// buffer is just a temporaray text container

    buffer << file.rdbuf(); // rdbuf is used to give all the contents of the file .so it means Take everything from file and Write it into buffer

        return buffer.str(); // convert the text in string and return
}



//Its entire job is: File Name -> Tell browser what type of file it is


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