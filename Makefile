CC=gcc
CFLAGS=-W -Wall -Werror -std=c11 -pedantic -Wno-unused-parameter

TARGET=timed-sunset

all: build
	./$(TARGET) --start 19:30 --stop 8

build:
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi

