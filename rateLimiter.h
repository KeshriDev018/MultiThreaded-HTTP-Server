#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H

#include <unordered_map>
#include <ctime>
#include <string>

using namespace std;


class RateLimiter
{

    unordered_map<string, pair<int, time_t>> ipRequests;

    static const int MAX_REQUESTS = 5; // Max requests allowed
    static const int TIME_WINDOW = 10; // Time window in seconds

public:
    
    bool isAllowed(const string &clientIP)
    {
        time_t now = time(0);

        // If IP not in map, first request from this IP
        if (ipRequests.find(clientIP) == ipRequests.end())
        {
            ipRequests[clientIP] = {1, now};
            return true;
        }

        auto &[requestCount, lastReset] = ipRequests[clientIP];

        // If time window has passed, reset counter
        if (now - lastReset >= TIME_WINDOW)
        {
            requestCount = 1;
            lastReset = now;
            return true;
        }

        // Within time window: check if limit exceeded
        if (requestCount < MAX_REQUESTS)
        {
            requestCount++;
            return true;
        }

        // Limit exceeded
        return false;
    }

 
    int getRemainingRequests(const string &clientIP)
    {
        time_t now = time(0);

        if (ipRequests.find(clientIP) == ipRequests.end())
        {
            return MAX_REQUESTS;
        }

        auto &[requestCount, lastReset] = ipRequests[clientIP];

        // Reset if window passed
        if (now - lastReset >= TIME_WINDOW)
        {
            return MAX_REQUESTS;
        }

        return max(0, MAX_REQUESTS - requestCount);
    }
};

#endif
