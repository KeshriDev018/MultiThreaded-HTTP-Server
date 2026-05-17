#include "router.h"

using namespace std;

/**
 * Handle route routing and file serving.
 * Returns:
 * - filePath
 * - statusCode
 * - statusText
 */
RouteResult handleRoute(const string &path)
{
    if (path == "/")
    {
        return {"static/index.html", 200, "OK"};
    }
    else if (path == "/about")
    {
        return {"static/about.html", 200, "OK"};
    }
    else if (path == "/style.css")
    {
        return {"static/style.css", 200, "OK"};
    }
    else if (path == "/api/status")
    {
        return {"static/status.json", 200, "OK"};
    }
    else
    {
        return {"static/404.html", 404, "Not Found"};
    }
}