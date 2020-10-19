#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct XMLElement XMLElement;
int find_doctype(FILE *file);
long get_size_of_file(FILE *file);
char *file_get_content(FILE *file);
void add_element(XMLElement* parent, XMLElement* child);
XMLElement* create_element(XMLElement* parent, int deepness);

struct XMLElement{
  XMLElement* parent;
  XMLElement** childs;
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

void add_element(XMLElement* parent, XMLElement* child){
  if(parent->childsCount == parent->childsCapacity){
    parent->childsCapacity *= 2;
    XMLElement** tab = malloc(sizeof(XMLElement)*parent->childsCapacity);
    for(int i = 0; i < parent->childsCount;i++){
      tab[i] = parent->childs[i];
    }
    parent->childs = tab;
  }
  parent->childsCount += 1;
  parent->childs[parent->childsCount] = child;
}

XMLElement* create_element(XMLElement* parent, int deepness){
  XMLElement* element = malloc(sizeof(XMLElement));
  if(parent != NULL){
    add_element(parent, element);
  }
  element->parent = parent;
  element->deepness = deepness;
  element->childsCount = 0;
  element->childsCapacity = 1;
  element->childs = malloc(sizeof(XMLElement));
}

