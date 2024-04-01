#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 12345              // Port number to bind the socket
#define BUFFER_SIZE 1024        // Size of the buffer for sending/receiving data
#define DEST_IP "192.168.0.123" // IP address of the destination IP

int main(void) {
  int sockfd;
  struct sockaddr_in src_addr, dest_addr;
  socklen_t src_addr_len = sizeof(src_addr);
  socklen_t dest_addr_len = sizeof(dest_addr);
  char buffer[BUFFER_SIZE];

  // Create a UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Initialize address structures
  memset(&src_addr, 0, sizeof(src_addr));
  memset(&dest_addr, 0, sizeof(dest_addr));
  src_addr.sin_family = AF_INET;
  src_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  src_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, DEST_IP, &dest_addr.sin_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }
  dest_addr.sin_family = AF_INET;
  dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
  dest_addr.sin_port = htons(PORT);

  // Bind the socket
  if (bind(sockfd, (struct sockaddr *)&src_addr, src_addr_len) == -1) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Echo node is running and waiting for messages...\n");

  while (1) {
    // Receive a message from the measure node
    ssize_t bytes_received =
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&dest_addr,
                 &dest_addr_len);
    if (bytes_received == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    printf("Message received from the measure node.\n");

    // Send a message to the measure node
    ssize_t bytes_sent =
        sendto(sockfd, buffer, BUFFER_SIZE, 0,
               (const struct sockaddr *)&dest_addr, dest_addr_len);
    if (bytes_sent == -1) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
    printf("Message sent to the measure node.\n");
  }

  // Close the socket
  close(sockfd);

  return 0;
}
