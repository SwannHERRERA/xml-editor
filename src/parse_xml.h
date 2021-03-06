#ifndef _parse_xml_h_
#define _parse_xml_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "parse_dtd.h"

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

typedef struct XML_element
{
  char *name;
  unsigned int number_of_attribute;
  xml_attribute_linkedlist *attributes;
  char *content;
  struct XML_element *parent;
  struct XML_element **childs;
  int childs_count;
  int childs_capacity;
  int deepness;
  bool autoclosing;
} XML_element;

void create_empty_xml_attribute_linkedlist(XML_element *element);
void print_attribute(XML_element *element);
void print_element(XML_element *element);
void get_content(char *subject, XML_element *element);
void free_element(XML_element *element);
XML_element *get_element(char *xml, char *root_name);
xml_attribute_linkedlist *create_next_element(XML_element *element);
void get_attr_value(int index, char *subject, xml_attribute *attr);
void get_attr_name(int index, char *subject, xml_attribute *attr);
int make_attributes(char *tag, char *subject, XML_element *element);
XML_element *parse_xml(char *xml);
bool is_closing_tag(char *s);
bool check_is_balise(char **xml);
bool check_is_doctype(char *s);
bool check_is_version(char *s);
bool check_is_comment(char *s);
bool is_autoclosing_tag(XML_element *element);
XML_element *get_next_element(char *xml, XML_element *parent, int deepness);
void realloc_childs(XML_element *element);
char *find_start(char *xml, XML_element *element);
xml_attribute **attributes_to_array(XML_element *element);

#endif