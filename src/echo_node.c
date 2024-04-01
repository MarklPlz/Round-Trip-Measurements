#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 12345       // Port number to bind the socket
#define BUFFER_SIZE 1024 // Size of the buffer for sending/receiving data

int main(void) {
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  socklen_t servaddr_len = sizeof(servaddr);
  socklen_t cliaddr_len = sizeof(cliaddr);
  char buffer[BUFFER_SIZE];

  // Create a UDP socket, SOCK_DGRAM for UDP
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Initialize address structures
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  // Bind the socket
  if (bind(sockfd, (struct sockaddr *)&servaddr, servaddr_len) == -1) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Echo node is running and waiting for messages...\n");

  while (1) {
    // Receive a message from the measure node
    ssize_t bytes_received =
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&cliaddr, &cliaddr_len);
    if (bytes_received == -1) {
      perror("recvfrom failed");
      exit(EXIT_FAILURE);
    }
    printf("Message received from the measure node.\n");

    // Send a message to the measure node
    ssize_t bytes_sent = sendto(sockfd, buffer, BUFFER_SIZE, MSG_CONFIRM,
                                (struct sockaddr *)&cliaddr, cliaddr_len);
    if (bytes_sent == -1) {
      perror("sendto failed");
      exit(EXIT_FAILURE);
    }
    printf("Message sent to the measure node.\n");
  }

  // Close the socket
  close(sockfd);

  return 0;
}
