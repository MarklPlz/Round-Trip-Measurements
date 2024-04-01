#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in dest_addr, src_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&src_addr, 0, sizeof(src_addr));

    // Filling server information
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    dest_addr.sin_port = htons(PORT);

    // Filling client information (for receiving)
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = INADDR_ANY;
    src_addr.sin_port = htons(0); // Let the OS choose a random port for receiving

    // Bind the socket with the client address (for receiving)
    if (bind(sockfd, (const struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;

    printf("Enter message to send: ");
    fgets(buffer, MAXLINE, stdin);

    sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&dest_addr, sizeof(dest_addr));
    printf("Message sent.\n");

    printf("Waiting for message...\n");
    len = sizeof(src_addr);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&src_addr, &len);
    buffer[n] = '\0';
    printf("Received message: %s\n", buffer);

    close(sockfd);
    return 0;
}

