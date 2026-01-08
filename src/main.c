#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define VERSION "v0.02"
#define BUFFER_SIZE 4096
#define BACKSPACE 8
#define ESCAPE 27


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
  int index = bufferLoad(file, buffer);
  fclose(file);
  while (1)
  {
    clearScreen();
    printf("##CURRENT FILE: \"%s\"\nINDEX<%d>, MODE = %s \nNORMAL MODE: 'i' = enter insert mode, 'q' = save and quit\nEXIT INSERT MODE USING: 'ESC'\n~\n~\n", filename, index, (mode==NORMAL)?"NORMAL":"INSERT");
    printf("%s", buffer);
    int key = _getch();
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
        buffer[index++] = '\n';
        buffer[index] = '\0';
        continue;
      }
      if (key == BACKSPACE && index > 0)
      {
        buffer[--index] = '\0';
        continue;
      }
      if (index < BUFFER_SIZE - 1)
      {
        buffer[index++] = (char)key;
        buffer[index] = '\0';
      }
    }
  }
  clearScreen();
  printf("## \"%s\" SAVED WITH CONTENTS:\n\n%s\n", filename, buffer);

  file = fopen(filename, "w");
  if (!file)
    return 1;
  fwrite(buffer, 1, (size_t)index, file);
  fclose(file);
  return 0;
}