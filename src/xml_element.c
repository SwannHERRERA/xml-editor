#include "xml_element.h"

XMLElement *create_element(XMLElement *parent)
{
  XMLElement *element = malloc(sizeof(XMLElement));
  if (element == NULL)
  {
    fprintf(stderr, "Failed to allocate memory [create_element]\n");
    exit(EXIT_FAILURE);
  }
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
  if (!element->childs)
  {
    fprintf(stderr, "Failed to allocate memory for {element->childs} [create_element]\n");
    exit(EXIT_FAILURE);
  }
  return element;
}

// TODO utiliser la fonction realloc()
void add_element(XMLElement *parent, XMLElement *child)
{
  if (parent->childsCount == parent->childsCapacity)
  {
    parent->childsCapacity *= 2;
    XMLElement **tab = malloc(sizeof(XMLElement) * parent->childsCapacity);
    if (tab == NULL)
    {
      fprintf(stderr, "Failed to allocate memory [add_element]\n");
      exit(EXIT_FAILURE);
    }
    for (int i = 0; i < parent->childsCount; i++)
    {
      tab[i] = parent->childs[i];
    }
    free(parent->childs);
    parent->childs = tab;
  }
  parent->childsCount += 1;
  parent->childs[parent->childsCount] = child;
}