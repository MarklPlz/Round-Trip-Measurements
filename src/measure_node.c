#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORT 12345               // Port number to bind the socket
#define SERVER_IP "192.168.0.28" // IP address of the destination server
#define SERVER_PORT 12345        // Port number of the destination server
#define INTERVAL 1               // Send interval

int main() {
  int i = 0;
  struct timespec start, end;
  double elapsed = 0;

  int sockfd;
  struct sockaddr_in server_addr; //, client_addr;
  // socklen_t client_addr_len = sizeof(client_addr);
  socklen_t server_addr_len = sizeof(server_addr);
  uint32_t buffer = 0;
  int BUFFER_SIZE = sizeof(buffer);

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
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  // Bind the socket to the specified port
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  printf("UDP server is running and waiting for messages...\n");

  while (i < 5) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Send a message to the server
    ssize_t bytes_sent =
        sendto(sockfd, &buffer, BUFFER_SIZE, 0,
               (const struct sockaddr *)&server_addr, server_addr_len);
    if (bytes_sent == -1) {
      perror("sendto");
      exit(EXIT_FAILURE);
    }
    printf("Message sent to the server.\n");

    // Receive message from server
    ssize_t bytes_received =
        recvfrom(sockfd, &buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&server_addr, &server_addr_len);
    if (bytes_received == -1) {
      perror("recvfrom");
      exit(EXIT_FAILURE);
    }
    printf("Message received from server.\n");

    clock_gettime(CLOCK_MONOTONIC, &end);

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
