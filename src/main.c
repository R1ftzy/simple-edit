#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define VERSION "v0.3.1"
#define BUFFER_SIZE 4096
#define BACKSPACE 8
#define ESCAPE 27
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

enum MODE
{
  NORMAL,
  INSERT
};
void clearScreen()
{
  system("cls");
  printf("##BASIC WINDOWS TUI TEXT EDITOR VERSION: %s\n\n", VERSION);
}

int bufferLoad(FILE *file, char *buffer)
{
  int c;
  int i = 0;
  while ((c = fgetc(file)) != EOF && i < BUFFER_SIZE - 1)
    buffer[i++] = (char)c;
  buffer[i] = '\0';
  return i;
}

void drawBuffer(char *buffer, int cursor)
{
  int len = (int)strlen(buffer);
  if (cursor==0)
    printf("|");
  for (int i = 0; i < len; i++)
  {
    printf("%c", buffer[i]);
    if (i == cursor-1)
      printf("|");
  }
}

void shiftRight(char *buffer)
{
  size_t len = strlen(buffer);
  for (int i = (int)len; i >= 0; i--)
  {
    buffer[i+1]=buffer[i];
  }
}

void shiftLeft(char *buffer)
{
  size_t len = strlen(buffer);
  for (size_t i = 0; i <= len; i++)
  {
    buffer[i]=buffer[i+1];
  }
}

int main(int argc, char *argv[])
{
  const char *filename = (argc >= 2) ? argv[1] : "untitled.txt";
  enum MODE mode = NORMAL;
  char buffer[BUFFER_SIZE];
  buffer[0] = '\0';
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    file = fopen(filename, "w+");
    if (!file)
      return 1;
  }
  int len = bufferLoad(file, buffer);
  int cursor = len;
  fclose(file);
  while (1)
  {
    clearScreen();
    printf("##CURRENT FILE: \"%s\"\nINDEX<%d>, MODE = %s \nNORMAL MODE: 'i' = enter insert mode, 'q' = save and quit\nEXIT INSERT MODE USING: 'ESC'\n~\n~\n", filename, cursor, (mode==NORMAL)?"NORMAL":"INSERT");
    drawBuffer(buffer, cursor);
    int key = _getch();
    if (key == 0 || key == 224)
    {
      int extended = _getch();
      switch (extended)
      {
      case ARROW_LEFT:
        if (cursor > 0)
          cursor--;
        break;
      case ARROW_RIGHT:
        if (cursor < (int)strlen(buffer))
          cursor++;
        break;
      }
      continue;
    }
    if (mode == NORMAL)
    {
      if (key == 'i')
        mode = INSERT;
      else if (key == 'q')
        break;
      continue;
    }
    else if (mode == INSERT)
    {
      if (key == ESCAPE)
      {
        mode = NORMAL;
        continue;
      }
      if (key == '\r')
      {
        if (cursor < BUFFER_SIZE - 1){
          shiftRight(buffer+cursor);
          buffer[cursor++] = '\n';
        }
        continue;
      }
      if (key == BACKSPACE && cursor > 0)
      {
        cursor--;
        shiftLeft(buffer+cursor);
        continue;
      }

      if (cursor < BUFFER_SIZE - 1 && key >= 32 && key <= 126)
      {
        shiftRight(buffer + cursor);
        buffer[cursor++] = (char)key;
      }
    }
  }
  clearScreen();
  printf("## \"%s\" SAVED WITH CONTENTS:\n\n%s\n", filename, buffer);

  file = fopen(filename, "w");
  if (!file)
    return 1;
  fwrite(buffer, 1, strlen(buffer), file);
  fclose(file);
  return 0;
}
