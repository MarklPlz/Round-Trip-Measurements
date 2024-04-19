#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PORT 12345             // Port number to bind the socket
#define DEST_IP "192.168.0.96" // IP address of the echo node server
#define INTERVAL 100 * 1e-6    // Send interval
#define PACKETS 50000          // Number of packets

int main(void) {
  struct timespec start, end;
  double elapsed;
  double measured_time[PACKETS];

  int sockfd;
  struct sockaddr_in servaddr;
  socklen_t servaddr_len = sizeof(servaddr);
  uint32_t buffer = 0;
  uint16_t buffer_size = sizeof(buffer);

  // Create a UDP socket, SOCK_DGRAM for UDP
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, DEST_IP, &servaddr.sin_addr) <= 0) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  printf("Measure node is running.\n");

  while (buffer < PACKETS) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Send a message to the echo node
    ssize_t bytes_sent = sendto(sockfd, &buffer, buffer_size, 0,
                                (struct sockaddr *)&servaddr, servaddr_len);
    if (bytes_sent == -1) {
      perror("sendto failed");
      exit(EXIT_FAILURE);
    }

    // Receive a message from the echo node
    ssize_t bytes_received =
        recvfrom(sockfd, &buffer, buffer_size, 0, (struct sockaddr *)&servaddr,
                 &servaddr_len);
    if (bytes_received == -1) {
      perror("recvfrom failed");
      exit(EXIT_FAILURE);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    measured_time[buffer] = elapsed * 1e6; // seconds to microseconds

    while (elapsed < INTERVAL) {
      clock_gettime(CLOCK_MONOTONIC, &end);
      // Calculate elapsed time
      elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    }

    buffer++;
  }

  // Close the socket
  close(sockfd);

  // Open file for writing
  FILE *data = fopen("./data/data.csv", "w");
  if (data == NULL) {
    printf("writing to file failed\n");
    return 1;
  }

  // Write data to the file
  fprintf(data, "Measured time [us]\n");
  for (int i = 0; i < PACKETS; ++i) {
    fprintf(data, "%f\n",
            measured_time[i]); // Write the value followed by a line break
  }

  return 0;
}
