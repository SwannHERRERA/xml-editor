#include <stdlib.h>
#include <stdio.h>

typedef struct XMLElement XMLElement;
XMLElement *create_element(XMLElement *parent);
void add_element(XMLElement *parent, XMLElement *child);

struct XMLElement
{
  XMLElement *parent;
  XMLElement **childs;
  char *name;
  int deepness;
  int childsCount;
  int childsCapacity;
  unsigned char occurenceFlag;
};