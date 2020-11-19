#include "file_helper.h"

void save_to_file(char *file_name, char *buffer)
{
  FILE *file = fopen(file_name, "w");
  if (file == NULL)
  {
    fprintf(stderr, "Unable to open file %s to save\n", file_name);
  }
  else
  {
    fwrite(buffer, sizeof(char), strlen(buffer), file);
  }
  fclose(file);
}

char *file_get_content(FILE *file)
{
  long size = get_size_of_file(file);
  char *buffer = (char *)malloc(sizeof(char) * size+1);
  if (buffer)
  {
    fread(buffer, sizeof(char), size, file);
    buffer[size] = 0;
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