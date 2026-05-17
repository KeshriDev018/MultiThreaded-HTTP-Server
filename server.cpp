#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main()
{
    // Create TCP socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        cerr << "Socket creation failed\n";
        return 1;
    }

    // Configure server address
    sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(serverSocket,
             (sockaddr *)&serverAddress,
             sizeof(serverAddress)) < 0)
    {
        cerr << "Bind failed\n";
        return 1;
    }

    // Start listening
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

        // Accept client connection
        int clientSocket = accept(
            serverSocket,
            (sockaddr *)&clientAddress,
            &clientSize);

        if (clientSocket < 0)
        {
            cerr << "Client connection failed\n";
            continue;
        }

        cout << "Client connected\n";

        // Basic HTTP response
        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "<html>"
            "<body>"
            "<h1>Hello from C++ HTTP Server</h1>"
            "</body>"
            "</html>";

        // Send response
        send(clientSocket,
             response.c_str(),
             response.size(),
             0);

        // Close client connection
        close(clientSocket);
    }

    close(serverSocket);

    return 0;
}