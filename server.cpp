#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <fstream>
#include <sstream>
#include "logger.h"
#include "fileHandler.h"
#include "rateLimiter.h"
#include "configManager.h"

using namespace std;

// Global rate limiter for all clients
RateLimiter rateLimiter;

/**
 * Handle route routing and file serving.
 * Returns: {filePath, statusCode, statusText}
 *
 * Routes:
 * GET / -> static/index.html
 * GET /about -> static/about.html
 * GET /style.css -> static/style.css
 * GET /* -> static/404.html (404)
 */
struct RouteResult
{
    string filePath;
    int statusCode;
    string statusText;
};

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
    else
    {
        // 404 Not Found
        return {"static/404.html", 404, "Not Found"};
    }
}

/**
 * Handle a single client connection
 * - Parse HTTP request
 * - Check rate limit
 * - Route request
 * - Send response
 * - Log request
 *
 * @param clientSocket - Socket file descriptor for client
 * @param clientIP - Client IP address
 */
void handleClient(int clientSocket, string clientIP)
{
    char buffer[4096] = {0};

    // Read request from client
    ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if (bytesRead <= 0)
    {
        cerr << "[ERROR] Failed to read from client socket\n";
        close(clientSocket);
        return;
    }

    cout << "\n===== HTTP REQUEST =====\n";
    cout << buffer << endl;

    // Convert request into string
    string request(buffer);

    string method = "GET";

    size_t firstSpace = request.find(" ");

    if (firstSpace != string::npos)
    {
        method = request.substr(0, firstSpace);
    }

    // Default path
    string path = "/";

    // Extract path from request
    size_t methodEnd = request.find(" ");
    size_t pathEnd = request.find(" ", methodEnd + 1);

    if (methodEnd != string::npos &&
        pathEnd != string::npos)
    {

        path = request.substr(
            methodEnd + 1,
            pathEnd - methodEnd - 1);
    }

    cout << "Requested Path: " << path << endl;
    cout << "Client IP: " << clientIP << endl;

    // Check rate limiting BEFORE processing request
    if (!rateLimiter.isAllowed(clientIP))
    {
        string html = "<html><body><h1>429 Too Many Requests</h1>"
                      "<p>Rate limit: 5 requests per 10 seconds.</p></body></html>";
        string response =
            "HTTP/1.1 429 Too Many Requests\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: " +
            to_string(html.size()) + "\r\n"
                                     "\r\n" +
            html;
        send(clientSocket, response.c_str(), response.size(), 0);
        logRequest(method, path, clientIP, 429, "Too Many Requests");
        close(clientSocket);
        return;
    }

    // Use refactored routing function
    RouteResult route = handleRoute(path);
    int statusCode = route.statusCode;
    string statusText = route.statusText;
    string filePath = route.filePath;

    string html;

    if (filePath != "")
    {
        html = readFile(filePath);

        if (html == "")
        {
            statusCode = 404;
            statusText = "Not Found";
            html = readFile("static/404.html");
            if (html == "")
            {
                html = "<html><body><h1>404 Not Found</h1></body></html>";
            }
        }
    }
    else
    {
        statusCode = 404;
        statusText = "Not Found";
        html = readFile("static/404.html");
        if (html == "")
        {
            html = "<html><body><h1>404 Not Found</h1></body></html>";
        }
    }
    // Build HTTP response
    string mimeType = getMimeType(filePath);

    string response =
        "HTTP/1.1 " + to_string(statusCode) + " " + statusText + "\r\n"
                                                                 "Content-Type: " +
        mimeType + "\r\n"
                   "Content-Length: " +
        to_string(html.size()) + "\r\n"
                                 "\r\n" +
        html;

    // Send response
    send(clientSocket, response.c_str(), response.size(), 0);

    // Log request WITH status code
    logRequest(method, path, clientIP, statusCode, statusText);

    // Close socket
    close(clientSocket);
}

int main()
{
    // Load configuration from config.txt
    ConfigManager::loadConfig();
    int port = ConfigManager::getPort();

    // Create TCP socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        cerr << "[ERROR] Socket creation failed\n";
        return 1;
    }

    // Configure server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);       // Convert port to network byte order
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

    // Bind socket to address and port
    if (bind(serverSocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cerr << "[ERROR] Bind failed on port " << port << "\n";
        return 1;
    }

    // Listen for incoming connections (backlog of 10)
    if (listen(serverSocket, 10) < 0)
    {
        cerr << "[ERROR] Listen failed\n";
        return 1;
    }

    cout << "[INFO] HTTP Server running on port " << port << "...\n";
    cout << "[INFO] Press Ctrl+C to stop\n";

    // Main event loop: accept and handle client connections
    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientSize = sizeof(clientAddress);

        // Block until a client connects
        int clientSocket = accept(serverSocket, (sockaddr *)&clientAddress, &clientSize);

        if (clientSocket < 0)
        {
            cerr << "[ERROR] Client connection failed\n";
            continue;
        }

        // Extract and log client IP address
        string clientIP = inet_ntoa(clientAddress.sin_addr);
        cout << "[CONNECT] New client connected from " << clientIP << "\n";

        // Spawn a new thread to handle this client
        // This allows the server to handle multiple clients concurrently
        thread clientThread(handleClient, clientSocket, clientIP);
        clientThread.detach(); // Detach thread (it will clean up automatically)
    }

    close(serverSocket);
    return 0;
}