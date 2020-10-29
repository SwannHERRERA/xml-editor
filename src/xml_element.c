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