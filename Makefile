# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -O3 -std=c18 -D_POSIX_C_SOURCE=199309L

# Source files
SRCS1 = src/echo_node.c
SRCS2 = src/measure_node.c

# Object files
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)

# Executables
TARGET1 = echo_node
TARGET2 = measure_node

# Default target
all: $(TARGET1) $(TARGET2)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executables
$(TARGET1): $(OBJS1)
	$(CC) $(OBJS1) -o $(TARGET1)
$(TARGET2): $(OBJS2)
	$(CC) $(OBJS2) -o $(TARGET2)

# Clean objects and executables
clean:
	rm -f $(OBJS1) $(OBJS2) $(TARGET1) $(TARGET2)

