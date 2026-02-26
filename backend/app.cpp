#include <iostream>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main() {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[30000] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    char hostname[1024];
    gethostname(hostname, 1024);

    std::string body = "Served by backend: " + std::string(hostname) + "\n";

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" +
        body;

    while (true) {

        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        read(new_socket, buffer, 30000);

        send(new_socket, response.c_str(), response.size(), 0);

        close(new_socket);
    }

    return 0;
}