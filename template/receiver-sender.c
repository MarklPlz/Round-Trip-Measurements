#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in src_addr, dest_addr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&src_addr, 0, sizeof(src_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));

    // Filling server information
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = INADDR_ANY;
    src_addr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;
    while (1) {
        printf("Waiting for message...\n");
        len = sizeof(dest_addr);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&dest_addr, &len);
        buffer[n] = '\0';
        printf("Received message: %s\n", buffer);

        printf("Enter message to send (or type 'exit' to quit): ");
        fgets(buffer, MAXLINE, stdin);

        if (strcmp(buffer, "exit\n") == 0)
            break;

        sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *)&dest_addr, len);
        printf("Message sent.\n");
    }

    close(sockfd);
    return 0;
}

