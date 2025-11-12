# Compiler
CC = gcc

# Directories
INCLUDE = include
SRC = src

# Target executable
TARGET = wcs_cmd_eth.exe

# Compiler flags
CFLAGS = -I$(INCLUDE) -Wall

# Get all .c files in src folder
SRCS = $(wildcard $(SRC)/*.c)

# Default rule
all:
	$(CC) $(SRCS) $(CFLAGS) -lws2_32 -o $(TARGET)

# Clean build files
clean:
	rm -f $(TARGET)

# Run the program
run: all
	./$(TARGET)
