#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    int server_id = socket(AF_INET, SOCK_STREAM, 0);
    if (server_id < 0)
    {
        perror("socket failed");
        return 1;
    }
    printf("socket created successfully: %d\n", server_id);

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8080);

    if (bind(server_id, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        return 1;
    }

    if (listen(server_id, 3) < 0)
    {
        perror("listen failed");
        return 1;
    }

    printf("server running successfully\n");

    while (1)
    {

        // three way handshake
        int addrlen = sizeof(address);
        int new_socket = accept(server_id, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0)
        {
            perror("accept failed");
            continue;
        }

        printf("connection accepted\n");

        // data transfer

        char buffer[1024] = {0};
        int valread = read(new_socket, buffer, sizeof(buffer));
        if (valread > 0)
            printf("data from client: %s\n", buffer);

        char *response = "Hello from Server\n";
        write(new_socket, response, strlen(response));

        // 4 way handshake

        close(new_socket);
        printf("connection closed\n");
    }

    close(server_id);
    return 0;
}
