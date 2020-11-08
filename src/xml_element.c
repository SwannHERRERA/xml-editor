#include "xml_element.h"

XMLElement *create_element(char *name)
{
  XMLElement *element = malloc(sizeof(XMLElement));
  if (element == NULL)
  {
    fprintf(stderr, "Failed to allocate memory [create_element]\n");
    exit(EXIT_FAILURE);
  }
  element->name = name;
  element->deepness = 0;
  element->childsCount = 0;
  element->childsCapacity = 20;
  element->childs = malloc(sizeof(XMLElement) * element->childsCapacity);
  if (!element->childs)
  {
    fprintf(stderr, "Failed to allocate memory for {element->childs} [create_element]\n");
    exit(EXIT_FAILURE);
  }
  return element;
}

void add_element(XMLElement *parent, XMLElement *child)
{
  if (parent->childsCount == parent->childsCapacity)
  {
    parent->childsCapacity *= 2;
    parent->childs = realloc(parent->childs, parent->childsCapacity);
    if (parent->childs == NULL)
    {
      fprintf(stderr, "Failed to allocate memory [add_element]\n");
      exit(EXIT_FAILURE);
    }
  }
  parent->childs[parent->childsCount] = child;
  parent->childsCount += 1;
}

void print_XMLElement(XMLElement *element)
{
  printf("Name: %s\n", element->name);
  if (element->parent != NULL)
  {
    printf("parent Name: %s\n", element->parent->name);
  }
  printf("profondeur: %d\n", element->deepness);
  printf("nombre d enfant: %d capacite: %d\n", element->childsCount, element->childsCapacity);
  printf("Caractère d'occurence: %c\n", element->occurenceFlag);
}

void free_DTD(XMLElement *root)
{
  free_XMLElement(root);
}

void free_XMLElement(XMLElement *element)
{
  for (int i = 0; i < element->childsCount; i += 1)
  {
    free_XMLElement(element->childs[i]);
  }
  free(element->name);
  free(element);
}