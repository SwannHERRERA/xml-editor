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

typedef struct XMLDTDElement XMLDTDElement;
typedef struct XMLAttribute XMLAttribute;
typedef enum AttributeValue AttributeValue;
typedef enum AttributeType AttributeType;
XMLDTDElement *create_element(char *name);
void print_attributes(XMLDTDElement *element);
void set_deepness(XMLDTDElement *parent);
void print_tree(XMLDTDElement *parent);
void add_element(XMLDTDElement *parent, XMLDTDElement *child);
void add_attribute(XMLDTDElement *element, char *name, AttributeValue value, AttributeType type);
void free_XMLElement(XMLDTDElement *element);
void free_DTD(XMLDTDElement *root);
XMLAttribute **attributes_dtd_to_array(XMLDTDElement *element);

struct XMLDTDElement
{
  XMLDTDElement *parent;
  XMLDTDElement **childs;
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