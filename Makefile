CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SRC_DIR = src
TARGET = rebbon

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC_DIR)/$(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/$(TARGET).c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
