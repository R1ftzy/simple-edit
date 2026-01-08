CC      := gcc
CFLAGS  := -Wall -Wextra -Wconversion -Wsign-conversion -Werror -pedantic
TARGET  := sedit
SRC     := src/main.c
OBJ     := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
