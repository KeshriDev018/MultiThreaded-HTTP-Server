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