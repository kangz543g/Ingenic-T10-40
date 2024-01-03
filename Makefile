# Makefile for led_control

CC = mipsel-openipc-linux-musl-gcc
CFLAGS = -Wall -O2
LDFLAGS =
STRIP = mipsel-openipc-linux-musl-strip

SRC = led_control.c
OBJ = $(SRC:.c=.o)
TARGET = led_control

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

strip: $(TARGET)
	$(STRIP) $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)