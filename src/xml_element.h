#include <stdlib.h>
#include <stdio.h>
// Occurence flag definitions
/*
 * At least one to many corresponds to +
 */
#define OCCURENCE_1_N 0b1
/*
 * 0 to N corresponds to *
 */
#define OCCURENCE_0_N 0b10
/*
 * 0 or 1 corresponds to ?
 */
#define OCCURENCE_0_1 0b100
/*
 * OR with the next element corresponds to |
 */
#define OCCURENCE_OR 0b1000


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