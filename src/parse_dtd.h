#ifndef _parse_dtd_h_
#define _parse_dtd_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "xml_element.h"
#include "file_helper.h"

char *find_doctype(char *buffer, char **root_name);
XMLDTDElement *parse_dtd(char *dtd, char *root_name);
XMLDTDElement *parse_element(char *node_name, char **buffer, int buffer_size);
XMLDTDElement *parse_sub_element(char *node_name, char **buffer, int buffer_size);
XMLDTDElement *complete_element(char **buffer, int buffer_size, int index, char *name);
void parse_attributes(XMLDTDElement *element, char **buffer, int buffer_size);
bool is_internal_doctype(char *doctype);
char *get_content_of_external_DTD(char *doctype);
char *get_DTD_filename(char *doctype);
long get_size_of_doctype(char *start);
int char_count(char *str, char character);
char *get_between_tokens(char *buffer, size_t *cursor, char *tokens);
char *get_node_childs(char *buffer, char *name, char *last_char);
bool is_xml_valid_char(char c);
char **split_string(char *dtd, int *size, char delim);
char *get_next_name(char *ptr_str, size_t *offset);
void set_child_occurence(char occurence_char, XMLDTDElement *child);
AttributeType get_attribute_type(char **str);
AttributeValue get_attribute_value(char **str);

#endif