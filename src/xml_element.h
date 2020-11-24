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

typedef struct DTD_element DTD_element;
typedef struct DTD_attribute DTD_attribute;
typedef enum AttributeValue AttributeValue;
typedef enum AttributeType AttributeType;
DTD_element *create_element(char *name);
void print_attributes(DTD_element *element);
void set_deepness(DTD_element *parent);
void print_tree(DTD_element *parent);
void add_element(DTD_element *parent, DTD_element *child);
void add_attribute(DTD_element *element, char *name, AttributeValue value, AttributeType type);
void free_XMLElement(DTD_element *element);
void free_DTD(DTD_element *root);
DTD_attribute **attributes_dtd_to_array(DTD_element *element);

struct DTD_element
{
  DTD_element *parent;
  DTD_element **childs;
  DTD_attribute *attributes;
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

struct DTD_attribute
{
  DTD_attribute *next;
  AttributeType type;
  AttributeValue value;
  char *name;
};

#endif