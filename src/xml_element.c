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
  element->attributes = NULL;
  element->occurenceFlag = 0;
  element->occurenceChar = 0;
  if (!element->childs)
  {
    fprintf(stderr, "Failed to allocate memory for {element->childs} [create_element]\n");
    exit(EXIT_FAILURE);
  }
  return element;
}

void set_deepness(XMLElement *element)
{
  if (element == NULL)
  {
    return;
  }
  for (int i = 0; i < element->childsCount; i++)
  {
    element->childs[i]->deepness = element->deepness + 1;
    set_deepness(element->childs[i]);
  }
}

void print_tree(XMLElement *element)
{
  if (element != NULL)
  {
    for (int i = 0; i < element->deepness; i++)
    {
      printf("\t");
    }
    printf("\u255A\u2550\u2550");
    printf(">%s %d\n", element->name, element->occurenceFlag);
    print_attributes(element);
    for (int i = 0; i < element->childsCount; i++)
    {
      print_tree(element->childs[i]);
    }
  }
}

void print_attributes(XMLElement *element)
{
  XMLAttribute *attrib = element->attributes;
  while (attrib != NULL)
  {
    for (int i = 0; i < element->deepness; i++)
    {
      printf("\t");
    }
    printf("%s\n", attrib->name);
    attrib = attrib->next;
  }
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
  child->parent = parent;
}

void add_attribute(XMLElement *element, char *name, AttributeValue value, AttributeType type)
{
  XMLAttribute *first = element->attributes;
  XMLAttribute *new = malloc(sizeof(XMLAttribute));
  new->next = first;
  element->attributes = new;
  new->name = name;
  new->type = type;
  new->value = value;
}

void free_DTD(XMLElement *root)
{
  free_XMLElement(root);
}

void free_XMLAttributes(XMLAttribute *attribute)
{
  XMLAttribute *swap;
  XMLAttribute *first = attribute;
  while (first != NULL)
  {
    swap = first->next;
    free(first->name);
    free(first);
    first = swap;
  }
}

void free_XMLElement(XMLElement *element)
{
  for (int i = 0; i < element->childsCount; i += 1)
  {
    free_XMLElement(element->childs[i]);
  }
  free_XMLAttributes(element->attributes);
  free(element->childs);
  free(element->name);
  free(element);
}

size_t count_attribute(XMLElement *element)
{
  size_t count = 0;
  XMLAttribute *first = element->attributes;
  while (first != NULL)
  {
    count += 1;
    first = first->next;
  }
  return count;
}