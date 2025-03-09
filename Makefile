CC=gcc
CFLAGS=-W -Wall -Werror -std=c11 -pedantic -Wno-unused-parameter

TARGET=timed-sunset

all: build
	./$(TARGET) --start 18:00 --stop 1:32

build:
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	if [ -f "$(TARGET)" ]; then rm $(TARGET); fi

