#ifndef _xml_element_h_
#define _xml_element_h_

#include <stdlib.h>
#include <stdio.h>
// Occurence flag definitions

/*
 * Just 1
 */
#define OCCURENCE_1_1 0b0
/*
 * At least one to many corresponds to +
 */
#define OCCURENCE_1_N 0b10
/*
 * 0 to N corresponds to *
 */
#define OCCURENCE_0_N 0b11
/*
 * 0 or 1 corresponds to ?
 */
#define OCCURENCE_0_1 0b1
/*
 * OR with the next element corresponds to |
 */
#define OCCURENCE_OR 0b1000

typedef struct DTD_Element DTD_Element;
typedef struct XMLAttribute XMLAttribute;
typedef enum AttributeValue AttributeValue;
typedef enum AttributeType AttributeType;
DTD_Element *create_element(char *name);
void print_attributes(DTD_Element *element);
void set_deepness(DTD_Element *parent);
void print_tree(DTD_Element *parent);
void add_element(DTD_Element *parent, DTD_Element *child);
void add_attribute(DTD_Element *element, char *name, AttributeValue value, AttributeType type);
void free_XMLElement(DTD_Element *element);
void free_DTD(DTD_Element *root);
XMLAttribute **attributes_dtd_to_array(DTD_Element *element);

struct DTD_Element
{
  DTD_Element *parent;
  DTD_Element **childs;
  XMLAttribute *attributes;
  char *name;
  int deepness;
  int childsCount;
  int childsCapacity;
  unsigned int numberOfAttribute;
  unsigned char occurenceFlag;
  char occurenceChar;
};

enum AttributeValue
{
  NONE,
  REQUIRED,
  IMPLIED,
  FIXED
};

enum AttributeType
{
  VALUE,
  CDATA
};

struct XMLAttribute
{
  XMLAttribute *next;
  AttributeType type;
  AttributeValue value;
  char *name;
};

#endif