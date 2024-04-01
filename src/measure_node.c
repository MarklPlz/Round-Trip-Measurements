#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORT 12345               // Port number to bind the socket
#define DEST_IP "192.168.0.95" // IP address of the destination server
#define DEST_PORT 12345        // Port number of the destination server
#define INTERVAL 1               // Send interval
#define BUFFER_SIZE 1024        // Size of the buffer for sending/receiving data


int main(void) {
  int i = 0;
  struct timespec start, end;
  double elapsed;

  int sockfd;
  struct sockaddr_in src_addr, dest_addr;
  socklen_t src_addr_len = sizeof(src_addr);
  socklen_t dest_addr_len = sizeof(dest_addr);
  //uint32_t buffer = 0;
  char buffer[] = "Hello, UDP server!";
  //int BUFFER_SIZE = sizeof(buffer);

  // Create a UDP socket
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Initialize server address structure
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

  // Bind the socket to the specified port
  if (bind(sockfd, (struct sockaddr *)&src_addr, src_addr_len) ==
      -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  printf("UDP server is running and waiting for messages...\n");

  while (i < 5) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Send a message to the server
    ssize_t bytes_sent = sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (bytes_sent == -1) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
    printf("Message sent to the server.\n");

    // Receive message from server
    ssize_t bytes_received =
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&dest_addr, &dest_addr_len);
    if (bytes_received == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    printf("Message received from server.\n");

    do {
      clock_gettime(CLOCK_MONOTONIC, &end);
      // Calculate elapsed time
      elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    } while (elapsed < INTERVAL);

    printf("Elapsed time: %f seconds\n", elapsed);

    i++;
  }

  // Close the socket
  close(sockfd);

  return 0;
}
