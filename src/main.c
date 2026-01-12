#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>

#define VERSION "v0.3.2"
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
  printf("##SIMPLE-EDIT VERSION: %s\n\n", VERSION);
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

int saveFile(const char *filename, char *buffer){
  FILE *file = fopen(filename, "w");
  if (!file)
    return 1;
  fwrite(buffer, 1, strlen(buffer), file);
  fclose(file);
  return 0;
}

int main(int argc, char *argv[])
{
  const char *filename = (argc >= 2) ? argv[1] : "untitled.txt";
  enum MODE mode = NORMAL;
  char buffer[BUFFER_SIZE];
  bool modified = false;
  int len;

  FILE *file = fopen(filename, "r");
  if (file)
  {
    len = bufferLoad(file, buffer);
    fclose(file);
  }
  else 
  {
    buffer[0]='\0';
    len = 0;
  }
  int cursor = len;

  int key;
  while (1)
  {
    clearScreen();
    printf("##CURRENT FILE: \"%s\" %s\nINDEX<%d/%d>, MODE = %s\n|--[i] INSERT mode, [esc] NORMAL mode, [s] save, [w] save and quit, [q] quit without saving\n~\n~\n", filename, (modified==true)?"(modified)":"", cursor, len, (mode==NORMAL)?"NORMAL":"INSERT");
    drawBuffer(buffer, cursor);
    key = _getch();
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
        if (cursor < len)
          cursor++;
        break;
      }
      continue;
    }
    if (mode == NORMAL)
    {
      if (key == 'i')
        mode = INSERT;
      else if (key == 'w')
        break;
      else if (key == 'q')
      {
        if (modified == true)
        {
          printf("\n\nUnsaved changes detected D:\n|--[q] Discard changes, [w] Save and exit, [s] Save and continue, [esc] Cancel: ");
          key = _getch();
          if (key == 'q' || key == 'w')
            break;
        }
        else break;
      }
      if (key == 's')
      {
        saveFile(filename, buffer);
        modified = false;
        continue;
      }
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
        modified = true;
        if (len < BUFFER_SIZE - 1){
          shiftRight(buffer+cursor);
          buffer[cursor++] = '\n';
          len++;
        }
        continue;
      }
      if (key == BACKSPACE && cursor > 0)
      {
        modified = true;
        len--;
        cursor--;
        shiftLeft(buffer+cursor);
        continue;
      }

      if (len < BUFFER_SIZE - 1 && key >= 32 && key <= 126)
      {
        len++;
        modified = true;
        shiftRight(buffer + cursor);
        buffer[cursor++] = (char)key;
      }
    }
  }
  clearScreen();
  if (key == 'w')
  {
    saveFile(filename, buffer);
    printf("## \"%s\" - saved successfully :D\n", filename);
  }
  else if (modified)
    printf("## \"%s\" - unsaved changes discarded >:C\n", filename);
  else
    printf("## \"%s\" - closed :P\n", filename);    
  return 0;
}
