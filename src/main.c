#include <ctype.h>
#include "parse_dtd.h"
#include "parse_xml.h"

xml_element *parse_xml(char *xml);
void print_element_tree(xml_element *root);

/**
 * I use ARGV to give relative path to xml
 */
int main(int argc, char **argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "Error attending xml file in parameters\n");
    return EXIT_FAILURE;
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
  {
    fclose(file);
    fprintf(stderr, "Error opening file %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  char *root_name;
  char *dtd_string = find_doctype(file, &root_name);
  XMLElement *dtd = parse_dtd(dtd_string, root_name);

  printf("\n\n   PARSE XML  \n\n");

  fseek(file, 0L, SEEK_SET);
  char *xml = file_get_content(file);
  parse_xml(xml);
  free_DTD(dtd);
  free(xml);
  fclose(file);
  free(dtd_string);
  free(root_name);
  return EXIT_SUCCESS;
}

void print_element_tree(xml_element *root)
{
  if (root == NULL)
  {
    return;
  }
}

void print_element_tree_recursive(xml_element *element)
{
  int i = 0, j = 0;
  xml_attribute_linkedlist *current_attr = element->attributes;
  for (i = 0; i < element->deepness; i += 1)
  {
    printf("  ");
  }
  printf("- %s\n", element->name);
  for (i = 0; i < element->deepness; i += 1)
  {
    printf("  ");
  }
  printf("  Attributes: %d\n", element->number_of_attribute);
  for (i = 0; i < element->number_of_attribute; i += 1)
  {
    for (j = 0; j < element->deepness; j += 1)
    {
      printf("  ");
    }
    printf("%s=\"%s\"\n", current_attr->value->name, current_attr->value->value);
    current_attr = current_attr->next;
  }
  for (i = 0; i < element->childs_count; i += 1)
  {
    print_element_tree_recursive(element->childs[i]);
  }
}

void realloc_childs(xml_element *element)
{
  xml_element **new_array = malloc(sizeof(xml_element *) * (element->childs_capacity * 2));
  element->childs_capacity *= 2;
  for (int i = 0; i < element->childs_count; i += 1)
  {
    new_array[i] = element->childs[i];
  }
  free(element->childs);
  element->childs = new_array;
}

/**
 * @param parent xml_element*
 * 
 */
xml_element *get_next_element(char *xml, xml_element *parent, int deepness)
{
  int i = 1;
  int start, end;
  char *name;
  long max_size = strlen(xml);
  while (!isspace(xml[i]) && xml[i] != '>' && i < max_size)
  {
    i += 1;
  }
  start = 1;
  end = i;
  name = malloc(sizeof(char) * (end - start));
  strncpy(name, xml + start, end - start);
  xml_element *element = get_element(xml, name);
  element->childs = malloc(sizeof(xml_element *) * 5);
  element->childs_count = 0;
  element->childs_capacity = 5;
  element->parent = parent;
  element->deepness = deepness;
  if (parent != NULL)
  {
    if (parent->childs_count == parent->childs_capacity)
    {
      realloc_childs(parent);
    }
    parent->childs[parent->childs_count] = element;
    parent->childs_count += 1;
  }
  print_element();
  free(name);
  return element;
}

/**
 * @param s string 
 * 
 */
bool check_is_comment(char *s)
{
  if (s[1] == '!')
  {

    return true;
  }
  return false;
}

bool check_is_version(char *s)
{
  if (s[1] == '?')
  {
    return true;
  }
  return false;
}

bool check_is_doctype(char *s)
{
  if (strncmp(s, "<!DOCTYPE ", 10) == 0)
  {
    return true;
  }
  return false;
}

bool check_is_balise(char **xml)
{
  int size_of_dtd;

  if (check_is_doctype(*xml) == true)
  {
    size_of_dtd = get_size_of_doctype(*xml);
    *xml = *xml + sizeof(char) * size_of_dtd;
    return false;
  }
  if (check_is_comment(*xml) == true)
  {
    *xml = strstr(*xml, "-->");
    return false;
  }
  if (check_is_version(*xml) == true)
  {
    *xml = strstr(*xml, "?>");
    return false;
  }
  return true;
}

bool is_closing_tag(char *s)
{
  if (s[1] == '/')
  {
    return true;
  }
  return false;
}

xml_element *parse_xml(char *xml)
{
  xml_element *root;
  xml_element *current_element = NULL;
  int deepness = 0;

  while ((xml = strchr(xml, '<')) != NULL)
  {
    if (!check_is_balise(&xml))
    {
      continue;
    }

    if (is_closing_tag(xml))
    {
      if (current_element == NULL)
      {
        fprintf(stderr, "Error closing tag when no one was open.\n");
        exit(EXIT_FAILURE);
      }
      deepness -= 1;
      current_element = current_element->parent;
    }
    else
    {
      current_element = get_next_element(xml, current_element, deepness);
      if (current_element->parent == NULL)
      {
        root = current_element;
      }
      deepness += 1;
    }
    xml = xml + sizeof(char) * 1;
  }
  print_element(current_element);
  return root;
}