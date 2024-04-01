#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345            // Port number to bind the socket
#define BUFFER_SIZE 1024      // Size of the buffer for sending/receiving data

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP server is running and waiting for messages...\n");

    // Receive message from client
    ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                       (struct sockaddr *)&client_addr, &client_addr_len);
    if (bytes_received == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';  // Add null terminator to make it a valid C string

    printf("Message received from client: %s\n", buffer);

    // Close the socket
    close(sockfd);

    return 0;
}

