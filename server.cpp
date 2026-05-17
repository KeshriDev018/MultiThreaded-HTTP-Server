#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "fileHandler.h"

using namespace std;

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket,
             (sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0)
    {
        cerr << "Bind failed\n";
        return 1;
    }

    if (listen(serverSocket, 10) < 0)
    {
        cerr << "Listen failed\n";
        return 1;
    }

    cout << "Server listening on port 8080...\n";

    while (true)
    {
        sockaddr_in clientAddress;
        socklen_t clientSize = sizeof(clientAddress);

        int clientSocket = accept(
            serverSocket,
            (sockaddr *)&clientAddress,
            &clientSize);

        if (clientSocket < 0)
        {
            cerr << "Client connection failed\n";
            continue;
        }

        char buffer[4096] = {0};

        read(clientSocket, buffer, sizeof(buffer));

        string request(buffer);

        string path = "/";

        size_t methodEnd = request.find(" ");
        size_t pathEnd = request.find(" ", methodEnd + 1);

        if (methodEnd != string::npos &&
            pathEnd != string::npos)
        {
            path = request.substr(
                methodEnd + 1,
                pathEnd - methodEnd - 1);
        }

        string html;

        if (path == "/")
        {
            html = readFile("static/index.html");
        }
        else if (path == "/about")
        {
            html = readFile("static/about.html");
        }
        else
        {
            html =
                "<html>"
                "<body>"
                "<h1>404 Not Found</h1>"
                "</body>"
                "</html>";
        }

        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n" +
            html;

        send(clientSocket,
             response.c_str(),
             response.size(),
             0);

        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}