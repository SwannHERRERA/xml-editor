#include "file_helper.h"

char *file_get_content(FILE *file)
{
  long size = get_size_of_file(file);
  char *buffer = (char *)malloc(sizeof(char) * size);
  if (buffer)
  {
    fread(buffer, sizeof(char), size, file);
  }
  else
  {
    fprintf(stderr, "Failed to allocate memory [file_get_content]\n");
    exit(EXIT_FAILURE);
  }
  return buffer;
}

long get_size_of_file(FILE *file)
{
  long size;
  long current_size = ftell(file);
  fseek(file, 0L, SEEK_END);
  size = ftell(file);
  fseek(file, 0L, SEEK_SET);
  fseek(file, current_size, SEEK_CUR);
  return size;
}