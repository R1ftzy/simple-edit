CC      := gcc
CFLAGS  := -O3 -Wall -Wextra -Wconversion -Wsign-conversion -Werror -pedantic
TARGET  := sedit
SRC     := src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

.PHONY: all