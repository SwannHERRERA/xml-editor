#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_doctype(FILE *file);
long get_size_of_file(FILE *file);
char *file_get_content(FILE *file);

/**
 * I use ARGV to give relative path to xml
 */
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Error attending xml file in parameters\n");
    return EXIT_FAILURE;
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
  {
    fclose(file);
    fprintf(stderr, "Error opening file %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  printf("%d\n", find_doctype(file));
  fclose(file);
}

// A terme j'aimerais bien que ça retourn
// <!DOCTYPE ESGI [<!ELEMENT classrooms (classroom+)><!ELEMENT classroom (#PCDATA)>]>
int find_doctype(FILE *file)
{
  char *buffer = file_get_content(file);
  if (strstr(buffer, "<!DOCTYPE") != NULL)
  {
    return 1;
  }
  return 0;
}

char *file_get_content(FILE *file)
{
  long size = get_size_of_file(file);
  char *buffer = malloc(sizeof(char) * size);
  if (buffer)
  {
    fread(buffer, sizeof(char), size, file);
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