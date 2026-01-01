#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(serverSocket < 0)
    {
        perror("socket");
        return 1;
    }

    std::cout << "Socket created\n";

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket,
            (sockaddr*)&serverAddr,
            sizeof(serverAddr)) < 0)
    {
        perror("bind");
        return 1;
    }

    std::cout << "Bind success\n";

    if(listen(serverSocket, 5) < 0)
    {
        perror("listen");
        return 1;
    }

    std::cout << "Listening on port 8080\n";

    int clientSocket =
        accept(serverSocket, nullptr, nullptr);

    if(clientSocket < 0)
    {
        perror("accept");
        return 1;
    }

    std::cout << "Client connected\n";

    char buffer[1024] = {};

    recv(clientSocket,
         buffer,
         sizeof(buffer),
         0);

    std::cout << "Received: "
              << buffer
              << '\n';

    close(clientSocket);
    close(serverSocket);
}