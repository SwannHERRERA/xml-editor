#include "xml_element.h"

DTD_element *create_element(char *name)
{
  DTD_element *element = malloc(sizeof(DTD_element));
  if (element == NULL)
  {
    fprintf(stderr, "Failed to allocate memory [create_element]\n");
    exit(EXIT_FAILURE);
  }
  element->name = name;
  element->deepness = 0;
  element->childsCount = 0;
  element->childsCapacity = 20;
  element->childs = malloc(sizeof(DTD_element) * element->childsCapacity);
  element->attributes = NULL;
  element->occurrenceFlag = 0;
  element->occurrenceChar = 0;
  element->numberOfAttribute = 0;
  if (!element->childs)
  {
    fprintf(stderr, "Failed to allocate memory for {element->childs} [create_element]\n");
    exit(EXIT_FAILURE);
  }
  return element;
}

void set_deepness(DTD_element *element)
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

void print_tree(DTD_element *element)
{
  if (element != NULL)
  {
    for (int i = 0; i < element->deepness; i++)
    {
      printf("\t");
    }
    printf("\u255A\u2550\u2550");
    printf(">%s %d\n", element->name, element->occurrenceFlag);
    print_attributes(element);
    for (int i = 0; i < element->childsCount; i++)
    {
      print_tree(element->childs[i]);
    }
  }
}

void print_attributes(DTD_element *element)
{
  DTD_attribute *attrib = element->attributes;
  while (attrib != NULL)
  {
    for (int i = 0; i < element->deepness; i++)
    {
      printf("\t");
    }
    printf("%s %d\n", attrib->name, attrib->value);
    attrib = attrib->next;
  }
}

void add_element(DTD_element *parent, DTD_element *child)
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

void add_attribute(DTD_element *element, char *name, AttributeValue value, AttributeType type)
{
  DTD_attribute *first = element->attributes;
  DTD_attribute *new = malloc(sizeof(DTD_attribute));
  new->next = first;
  element->attributes = new;
  new->name = name;
  new->type = type;
  new->value = value;

  element->numberOfAttribute += 1;
}

DTD_attribute **attributes_dtd_to_array(DTD_element *element)
{
  DTD_attribute **res = malloc(sizeof(DTD_attribute *) * element->numberOfAttribute);
  DTD_attribute *head = element->attributes;

  for (unsigned int i = 0; i < element->numberOfAttribute; i += 1)
  {
    res[i] = head;
    head = head->next;
  }
  return res;
}

void free_DTD(DTD_element *root)
{
  free_XMLElement(root);
}

void free_XMLAttributes(DTD_attribute *attribute)
{
  DTD_attribute *swap;
  DTD_attribute *first = attribute;
  while (first != NULL)
  {
    swap = first->next;
    free(first->name);
    free(first);
    first = swap;
  }
}

void free_XMLElement(DTD_element *element)
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
