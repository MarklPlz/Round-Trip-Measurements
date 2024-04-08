#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PORT 12345          // Port number to bind the socket
#define DEST_IP "127.0.0.1" // IP address of the echo node server
#define INTERVAL 10 * 1e-6  // Send interval
#define PACKETS 1000        // Number of packetsa

int main(void) {
  struct timespec start, end;
  double elapsed;
  uint32_t measured_time[PACKETS];

  int sockfd;
  struct sockaddr_in servaddr;
  socklen_t servaddr_len = sizeof(servaddr);
  uint32_t buffer = 0;
  uint16_t buffer_size = sizeof(buffer);

  // Open file for writing
  FILE *data = fopen("data.csv", "w");
  if (data == NULL) {
    printf("Fehler beim Öffnen der Datei.\n");
    return 1;
  }

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
    measured_time[buffer] = (end.tv_nsec - start.tv_nsec);

    do {
      clock_gettime(CLOCK_MONOTONIC, &end);
      // Calculate elapsed time
      elapsed =
          (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    } while (elapsed < INTERVAL);

    buffer++;
  }

  // Close the socket
  close(sockfd);

  // Write data to the file
  fprintf(data, "Measured time [ns]\n");
  for (int i = 0; i < PACKETS; ++i) {
    fprintf(data, "%u\n",
            measured_time[i]); // Write the value followed by a line break
  }

  // print measured times
  /*for (int i = 0; i < PACKETS; i++) {
    printf("%d ", measured_time[i]);
    printf("\n");
  }*/

  return 0;
}
