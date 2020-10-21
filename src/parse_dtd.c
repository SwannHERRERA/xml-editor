#include "parse_dtd.h"

// TODO attention http
char *find_doctype(FILE *file)
{

  char *buffer = file_get_content(file);
  char *start = strstr(buffer, "<!DOCTYPE");
  char size_of_doctype = get_size_of_doctype(start);
  char *doctype = (char *)malloc(sizeof(char) * size_of_doctype);
  if (!doctype)
  {
    fprintf(stderr, "Failed to allocate memory for {doctype} [find_doctype]");
    exit(EXIT_FAILURE);
  }
  strncpy(doctype, start, size_of_doctype);
  free(buffer);
  if (is_internal_doctype(doctype))
  {
    return doctype;
  }
  else
  {
    return get_content_of_external_DTD(doctype);
  }
}

char *get_content_of_external_DTD(char *doctype)
{
  char *res = NULL;
  char *external_DTD_filename = NULL;
  FILE *external_DTD_file = NULL;

  external_DTD_filename = get_DTD_filename(doctype);
  external_DTD_file = fopen(external_DTD_filename, "r");

  if (external_DTD_file == NULL)
  {
    fprintf(stderr, "Failed to open file: %s\n", external_DTD_filename);
    fclose(external_DTD_file);
    exit(EXIT_FAILURE);
  }

  res = file_get_content(external_DTD_file);
  free(external_DTD_filename);
  free(doctype);
  fclose(external_DTD_file);
  return res;
}

char *get_DTD_filename(char *doctype)
{
  char *res;
  long size_of_doctype = strlen(doctype);
  int n = 0;
  int start = 0;
  int end = 0;
  while (n < size_of_doctype)
  {
    if (doctype[n] == '"')
    {
      if (start == 0)
      {
        start = n + 1;
      }
      else
      {
        end = n;
        break;
      }
    }
    n += 1;
  }
  res = (char *)malloc(sizeof(char) * (end - start));
  if (res == NULL)
  {
    fprintf(stderr, "Memory allocation failed [get_DTD_filename]");
    exit(EXIT_FAILURE);
  }
  strncpy(res, doctype + start, end - start);
  return res;
}

long get_size_of_doctype(char *start)
{
  long size_of_buffer = strlen(start);
  long n = 1;
  int nb_open_tags = 0;
  int exit_loop = 0;

  while (n < size_of_buffer && exit_loop == 0)
  {
    if (start[n] == '<')
    {
      nb_open_tags += 1;
    }
    else if (start[n] == '>')
    {
      if (nb_open_tags == 0)
      {
        exit_loop = 1;
      }
      else
      {
        nb_open_tags -= 1;
      }
    }
    n += 1;
  }
  return n;
}

bool is_internal_doctype(char *doctype)
{
  if (strstr(doctype, "SYSTEM") == NULL)
  {
    return true;
  }
  return false;
}

int char_count(char *str, char character)
{
  int counter = 0;
  for (size_t i = 0; i < strlen(str); i++)
  {
    if (str[i] == character)
    {
      counter++;
    }
  }
  return counter;
}

char **split_string(char *dtd, int *size)
{
  printf("Starting to parse dtd\n");
  *size = char_count(dtd, '>');
  char **buffer = malloc(sizeof(char *) * (*size));
  if (buffer == NULL)
  {
    fprintf(stderr, "Failed to allocate memory [parse_dtd]\n");
    exit(EXIT_FAILURE);
  }
  char tmp[strlen(dtd)];
  strcpy(tmp, dtd);
  int i = 0;
  buffer[i] = strtok(tmp, ">");
  while (buffer[i] != NULL)
  {
    buffer[++i] = strtok(NULL, ">");
  }
  return buffer;
}

XMLElement *create_elements_tree(char **buffer, int buffer_size)
{
  for (int i = 0; i < buffer_size; i++)
  {
    char *ptr_str = strstr(buffer[i], "<!ELEMENT ");
    if (ptr_str != NULL)
    {
      int j = strlen("<!ELEMENT ");
      char name[255];
      bool found = false;
      char *name_start = NULL;
      int name_length = 0;
      while (ptr_str[j] != ' ' || !found)
      {
        if (ptr_str[j] != ' ' && !found)
        {
          found = true;
          name_start = buffer[i] + j;
        }
        if (ptr_str[j] != ' ')
        {
          name_length++;
        }
        j++;
      }
      strncpy(name, name_start, name_length);
      name[name_length] = 0;
      printf("%s\n", name);
    }
  }
  return NULL;
}

XMLElement *parse_dtd(char *dtd)
{
  int buffer_size = 0;
  char **buffer = split_string(dtd, &buffer_size);
  XMLElement *parent = create_elements_tree(buffer, buffer_size);
  free(buffer);
  return parent;
}