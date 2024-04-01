#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PORT 12345          // Port number to bind the socket
#define DEST_IP "127.0.0.1" // IP address of the echo node server
#define INTERVAL 1          // Send interval
#define BUFFER_SIZE 1024    // Size of the buffer for sending/receiving data

int main(void) {
  int i = 0;
  struct timespec start, end;
  double elapsed;

  int sockfd;
  struct sockaddr_in servaddr;
  socklen_t servaddr_len = sizeof(servaddr);
  char buffer[] = "Hello, UDP server!";

  // Create a UDP socket, SOCK_DGRAM for UDP
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);

  while (i < 5) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Send a message to the echo node
    ssize_t bytes_sent = sendto(sockfd, buffer, BUFFER_SIZE, 0,
                                (struct sockaddr *)&servaddr, servaddr_len);
    if (bytes_sent == -1) {
      perror("sendto failed");
      exit(EXIT_FAILURE);
    }
    printf("Message sent to the echo node.\n");

    // Receive a message from the echo node
    ssize_t bytes_received =
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                 (struct sockaddr *)&servaddr, &servaddr_len);
    if (bytes_received == -1) {
      perror("recvfrom failed");
      exit(EXIT_FAILURE);
    }
    printf("Message received from the echo node.\n");

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
