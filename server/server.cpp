#include "server.h"
#include "request_handler.h"

#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>

void handleClient(int clientSocket)
{
    char buffer[1024] = { 0 };

    read(clientSocket, buffer, 1024);
    std::string request(buffer);

    std::cout << "Client: " << request << std::endl;

    std::string response = handleRequest(request);

    send(clientSocket, response.c_str(), response.size(), 0);
    close(clientSocket);
}

void startServer(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server started on port " << port << std::endl;

    while (true)
    {
        int clientSocket = accept(server_fd, nullptr, nullptr);
        std::thread(handleClient, clientSocket).detach();
    }
}