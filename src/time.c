#include <stdio.h>
#include <time.h>

int main() {
    struct timespec start, end;
    double elapsed = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    
    printf("started\n");

    clock_gettime(CLOCK_MONOTONIC, &end);
    
    while(elapsed < 1)
    {
    // Calculate elapsed time
    elapsed = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / 1e9;
    clock_gettime(CLOCK_MONOTONIC, &end);
    }

    printf("Elapsed time: %f seconds\n", elapsed);

    return 0;
}

