CC      = gcc
CFLAGS  = -ansi -pedantic -Wall
TARGET  = archeology
SRC     = archeology.c

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o