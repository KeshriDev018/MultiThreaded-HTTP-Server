#ifndef ROUTER_H
#define ROUTER_H

#include <string>

using namespace std;

struct RouteResult
{
    string filePath;
    int statusCode;
    string statusText;
};

RouteResult handleRoute(const string &path);

#endif