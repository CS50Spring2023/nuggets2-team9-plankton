CC = gcc
CFLAGS = -Wall -Wextra

# Source files
SRCS = vistest.c message.c file.c log.c mem.c grid.c game.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = vistest

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)