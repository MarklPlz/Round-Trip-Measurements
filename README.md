# Round-Trip-Measurements

communication timing measurements.

## Build Process

From your command line:

```bash
# Clone this repository
$ git clone https://github.com/MarklPlz/Round-Trip-Measurements.git

# Go into the repository
$ cd Round-Trip-Measurements

# Compile the source files
$ gcc -Wall -Wextra -pedantic -O3 -std=c18 src/echo_node.c -o echo_node
$ gcc -Wall -Wextra -pedantic -O3 -std=c18 src/measure_node.c -o measure_node -D_POSIX_C_SOURCE=199309L

# Run the program
$ ./echo_node
$ ./measure_node
```
