CC = gcc-13
CFLAGS = -std=c11 -Wall -Wextra -pedantic

TARGET = userclean
SRC = userclean.c

.PHONY: all clean valgrind

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

valgrind: $(TARGET)
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

clean:
	rm -f $(TARGET)
