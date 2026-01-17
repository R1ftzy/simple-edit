#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define VERSION "v0.4.0"
#define BUFFER_SIZE 4096
#define BACKSPACE 8
#define ESCAPE 27
#define ARROW_LEFT 75
#define ARROW_RIGHT 77


typedef enum
{
  NONE,
  QUIT,
  SAVE_AND_QUIT,
} ExitType;

typedef enum
{
  NORMAL,
  INSERT
} Mode;

typedef struct {
  char buffer[BUFFER_SIZE];
  int len;
  int cursor;
  bool modified;
  Mode mode;
  bool exit;
  ExitType exit_type;
  const char *filename;
} Editor;

bool enable_ansi();
bool ansi_support = false;
void clearScreen();
int bufferLoad(FILE *file, char *buffer);
void drawBuffer(char *buffer, int cursor, int len);
void shiftRight(char *buffer, int len);
void shiftLeft(char *buffer, int len);
int saveFile(const char *filename, char *buffer, int len);
void handleInput(Editor *E, int key);

int main(int argc, char *argv[])
{
  ansi_support=enable_ansi();
  if(ansi_support==true){
    printf("\033[?1049h");
    fflush(stdout);
  }
  Editor E;
  E.filename = (argc >= 2) ? argv[1] : "untitled.txt";
  E.mode = NORMAL;
  E.modified = false;
  E.exit = false;
  E.exit_type = NONE;
  FILE *file = fopen(E.filename, "r");
  if (file)
  {
    E.len = bufferLoad(file, E.buffer);
    fclose(file);
  }
  else 
  {
    E.buffer[0]='\0';
    E.len = 0;
  }
  E.cursor = E.len;

  int key;
  while (!E.exit)
  {
    clearScreen();
    printf("##CURRENT FILE: \"%s\" %s\nINDEX<%d/%d>, MODE = %s\n|--[i] INSERT mode, [esc] NORMAL mode, [s] save, [w] save and quit, [q] quit without saving\n~\n~\n", E.filename, (E.modified==true)?"(modified)":"", E.cursor, E.len, (E.mode==NORMAL)?"NORMAL":"INSERT");
    drawBuffer(E.buffer, E.cursor, E.len);
    key = _getch();
    handleInput(&E, key);
  }
  if(ansi_support==true){
    printf("\033[?1049l");
    fflush(stdout);
  }
  printf("##SIMPLE-EDIT VERSION: %s\n\n", VERSION);
  if (E.exit_type == SAVE_AND_QUIT)
  {
    saveFile(E.filename, E.buffer, E.len);
    printf("## \"%s\" - saved successfully :D\n", E.filename);
  }
  else if (E.exit_type == QUIT)
    printf("## \"%s\" - unsaved changes discarded >:C\n", E.filename);
  else if (E.exit_type == NONE)
    printf("## \"%s\" - closed :P\n", E.filename);    


  return 0;
}






bool enable_ansi()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return false;

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode))
        return false;

    if (!SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
        return false;

    return true;
}
void handleInput(Editor *E, int key)
{
  if (key == 0 || key == 224)
  {
    int extended = _getch();
    switch (extended)
    {
    case ARROW_LEFT:
      if (E->cursor > 0)
        E->cursor--;
      break;
    case ARROW_RIGHT:
      if (E->cursor < E->len)
        E->cursor++;
      break;
    }
    return;
  }
  if (E->mode == NORMAL)
  {
    if (key == 'i')
      E->mode = INSERT;
    else if (key == 'w'){
      E->exit_type = SAVE_AND_QUIT;
      E->exit=true;
      return;
    }
    else if (key == 'q')
    {
      if (E->modified == true)
      {
        printf("\n\nUnsaved changes detected D:\n|--[q] Discard changes, [w] Save and exit, [s] Save and continue, [esc] Cancel: ");
        key = _getch();
        if (key == 'q'){
          E->exit_type = QUIT;
          E->exit=true;
          return;
        }
        if (key == 'w'){
          E->exit_type = SAVE_AND_QUIT;
          E->exit=true;
          return;
        }
      }
      else{
        E->exit=true;
        return;
      }
    }
    if (key == 's')
    {
      saveFile(E->filename, E->buffer, E->len);
      E->modified = false;
    }
    return;
  }
  else if (E->mode == INSERT)
  {
    if (key == ESCAPE)
    {
      E->mode = NORMAL;
      return;
    }
    if (key == '\r')
    {
      E->modified = true;
      if (E->len < BUFFER_SIZE - 1)
      {
        shiftRight(E->buffer + E->cursor, E->len - E->cursor);
        E->buffer[E->cursor++] = '\n';
        E->len++;
      }
      return;
    }
    if (key == BACKSPACE && E->cursor > 0)
    {
      E->modified = true;
      E->len--;
      E->cursor--;
      shiftLeft(E->buffer + E->cursor, E->len - E->cursor);
      return;
    }

    if (E->len < BUFFER_SIZE - 1 && key >= 32 && key <= 126)
    {
      E->modified = true;
      shiftRight(E->buffer + E->cursor, E->len - E->cursor);
      E->buffer[E->cursor++] = (char)key;
      E->len++;
    }
  }
}

void clearScreen()
{
  if(ansi_support==true){
    printf("\033[H\033[2J");
  }
  else{
    system("cls");
  }
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

void drawBuffer(char *buffer, int cursor, int len)
{
  if (cursor==0)
    printf("|");
  for (int i = 0; i < len; i++)
  {
    printf("%c", buffer[i]);
    if (i == cursor-1)
      printf("|");
  }
}

void shiftRight(char *buffer, int len)
{
  for (int i = len; i >= 0; i--)
  {
    buffer[i+1]=buffer[i];
  }
}

void shiftLeft(char *buffer, int len)
{
  for (int i = 0; i <= len; i++)
  {
    buffer[i]=buffer[i+1];
  }
}

int saveFile(const char *filename, char *buffer, int len){
  FILE *file = fopen(filename, "w");
  if (!file)
    return 1;
  fwrite(buffer, 1, (size_t)len, file);
  fclose(file);
  return 0;
}
