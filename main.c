#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct XMLElement XMLElement;
typedef struct Doctype Doctype;
char *find_doctype(FILE *file);
long get_size_of_file(FILE *file);
char *file_get_content(FILE *file);
void add_element(XMLElement *parent, XMLElement *child);
XMLElement *create_element(XMLElement *parent);
XMLElement *parse_dtd(char *dtd);
long get_size_of_doctype(char *start);
int is_internal_doctype(char *doctype);
char *get_content_of_external_DTD(char *doctype);
char *get_DTD_filename(char *doctype);

struct Doctype
{
  char *name;
  XMLElement *elements;
};

struct XMLElement
{
  XMLElement *parent;
  XMLElement **childs;
  char* name;
  int deepness;
  int childsCount;
  int childsCapacity;
};

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
  printf("%s\n", find_doctype(file));
  fclose(file);
  parse_dtd("<!ELEMENT classrooms (classroom+)><!ELEMENT classroom (#PCDATA)>");
}

// C'est ici qu'on derterminera si la DTD est dans un fichier externe ou pas attention http
char *find_doctype(FILE *file)
{

  char *buffer = file_get_content(file);
  char *start = strstr(buffer, "<!DOCTYPE");
  char size_of_doctype = get_size_of_doctype(start);
  char *doctype = (char *)malloc(sizeof(char) * size_of_doctype);
  strncpy(doctype, start, size_of_doctype);

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

  free(external_DTD_filename);
  return file_get_content(external_DTD_file);
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

// TODO mettre en BOOL
int is_internal_doctype(char *doctype)
{
  if (strstr(doctype, "SYSTEM") == NULL)
  {
    return 1;
  }
  return 0;
}

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

// TODO utiliser la fonction realloc()
void add_element(XMLElement *parent, XMLElement *child)
{
  if (parent->childsCount == parent->childsCapacity)
  {
    parent->childsCapacity *= 2;
    XMLElement **tab = malloc(sizeof(XMLElement) * parent->childsCapacity);
    for (int i = 0; i < parent->childsCount; i++)
    {
      tab[i] = parent->childs[i];
    }
    parent->childs = tab;
  }
  parent->childsCount += 1;
  parent->childs[parent->childsCount] = child;
}

XMLElement *create_element(XMLElement *parent)
{
  XMLElement *element = malloc(sizeof(XMLElement));
  element->deepness = 0;
  if (parent != NULL)
  {
    add_element(parent, element);
    element->deepness = parent->deepness + 1;
  }
  element->parent = parent;
  element->childsCount = 0;
  element->childsCapacity = 20;
  element->childs = malloc(sizeof(XMLElement));
  return element;
}

XMLElement *parse_dtd(char *dtd)
{
  XMLElement *parent = NULL;
  char **buff = malloc(sizeof(char*)*20);
  int i = 0;
  buff[i]= strtok(dtd, ">");
  while(buff[i] != NULL){
    buff[++i] = strtok(NULL, ">");
  }
  for(int i = 0; i < 20;i++){
    printf("%s", buff[i]);
  }
  return parent;
}
