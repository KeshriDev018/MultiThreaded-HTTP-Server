#ifndef LOGGER_H
#define LOGGER_H

#include <string>

using namespace std;

/**
 * Log HTTP request with response status
 * Format: [YYYY-MM-DD HH:MM:SS] CLIENT_IP METHOD PATH STATUS_CODE STATUS_TEXT
 * Example: [2026-05-17 02:10:15] 127.0.0.1 GET /about 200 OK
 */
void logRequest(const string &method,
                const string &path,
                const string &clientIP,
                int statusCode,
                const string &statusText);

#endif