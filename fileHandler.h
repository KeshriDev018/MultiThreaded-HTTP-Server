#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>

using namespace std;

string readFile(const string &filePath);

string getMimeType(const string &path);

#endif