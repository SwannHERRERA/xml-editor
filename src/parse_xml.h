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

typedef struct xml_element
{
  char *name;
  int number_of_attribute;
  xml_attribute_linkedlist *attributes;
  char *content;
  struct xml_element *parent;
  struct xml_element **childs;
  int childs_count;
  int childs_capacity;
  int deepness;
} xml_element;

void create_empty_xml_attribute_linkedlist(xml_element *element);
void print_attribute(xml_element *element);
void print_element(xml_element *element);
void get_content(char *subject, xml_element *element);
void free_element(xml_element *element);
xml_element *get_element(char *xml, char *root_name);
xml_attribute_linkedlist *create_next_element(xml_attribute_linkedlist *head);
int make_attributes(char *tag, char *subject, xml_element *element);
xml_element *parse_xml(char *xml);
bool is_closing_tag(char *s);
bool check_is_balise(char **xml);
bool check_is_doctype(char *s);
bool check_is_version(char *s);
bool check_is_comment(char *s);
xml_element *get_next_element(char *xml, xml_element *parent, int deepness);
void realloc_childs(xml_element *element);
char *found_start(char *xml, xml_element *element);

#endif