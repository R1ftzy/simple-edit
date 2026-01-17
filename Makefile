CC      := gcc
TARGET  := sedit.exe
SRC     := src/main.c

CFLAGS_DEV  := -g -O0 -Wall -Wextra -Wconversion -Wsign-conversion -Werror -pedantic
CFLAGS_RELEASE := -O2 -s -Wall -Wextra -Wconversion -Wsign-conversion -Werror -pedantic

all: release

dev:
	$(CC) $(CFLAGS_DEV) $(SRC) -o $(TARGET)

release:
	$(CC) $(CFLAGS_RELEASE) $(SRC) -o $(TARGET)

.PHONY: all dev release