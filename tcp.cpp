#include <iostream>
#include <unistd.h>     // for close, read, write
#include <arpa/inet.h>  // for inet_addr
#include <netinet/in.h> // for sockaddr_in
#include <cstring>

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Socket creation failed\n";
        return 1;
    }
    std::cout << "Socket created successfully\n";

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8800);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind failed\n";
        return 1;
    }
    std::cout << "Bind successful\n";

    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listen failed\n";
        return 1;
    }
    std::cout << "Server listening on port 8800\n";

    while (true)
    {
        socklen_t addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0)
        {
            std::cerr << "Accept failed\n";
            continue;
        }
        std::cout << "Connection accepted\n";

        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, sizeof(buffer) - 1);
        if (valread > 0)
        {
            buffer[valread] = '\0'; // null-terminate
            std::cout << "Data from client: " << buffer << "\n";
        }

        const char *response = "Hello from Server\n";
        write(new_socket, response, strlen(response));

        close(new_socket);
        std::cout << "Connection closed\n";
    }

    close(server_fd);
    return 0;
}
