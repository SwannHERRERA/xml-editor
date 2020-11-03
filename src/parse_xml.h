#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct xml_attribute
{
  char *name;
  char *value;
} xml_attribute;

typedef struct xml_attribute_linkedlist xml_attribute_linkedlist;

struct xml_attribute_linkedlist
{
  xml_attribute *value;
  xml_attribute_linkedlist *next;
};

typedef struct xml_element
{
  char *name;
  unsigned int number_of_attribute;
  xml_attribute_linkedlist *attributes;
  char *content;
} xml_element;

void create_empty_xml_attribute_linkedlist(xml_element *element);
void print_attribute(xml_element *element);
void print_element(xml_element *element);
void get_content(char *subject, xml_element *element);
void free_element(xml_element *element);
xml_element *get_element(char *xml, char *root_name);
xml_attribute_linkedlist *create_next_element(xml_attribute_linkedlist *head);
int make_attributes(char *tag, char *subject, xml_element *element);