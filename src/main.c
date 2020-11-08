#include <ctype.h>
#include "parse_dtd.h"
#include "parse_xml.h"

xml_element *parse_xml(char *xml);

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

/**
 * @param prev element
 * 
 */
xml_element *get_next_element(char *xml)
{
  printf("get_next_element\n");
  int i = 1;
  int start, end;
  char *name;
  long max_size = strlen(xml);
  printf("max_size: %ld\n", max_size);
  while (!isspace(xml[i]) && xml[i] != '>' && i < max_size)
  {
    i += 1;
  }
  start = 1;
  end = i;
  name = malloc(sizeof(char) * (end - start));
  strncpy(name, xml + start, end - start);
  printf("name: %s\n", name);
  xml_element *element = get_element(xml, name);
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
  if (strstr(s, "<!DOCTYPE ") == NULL)
  {
    return false;
  }
  return true;
}

bool check_is_balise(char **xml)
{
  int size_of_dtd;

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
  if (check_is_doctype(*xml) == true)
  {
    printf("it's doctype\n");
    size_of_dtd = get_size_of_doctype(*xml);
    *xml = *xml + sizeof(char) * size_of_dtd;
    return false;
  }
  return true;
}

bool is_closing_tag(char *s, xml_element *element)
{
  if (element == NULL)
  {
    fprintf(stderr, "Error element is NULL\n");
    exit(EXIT_FAILURE);
  }
  char closing_tag[strlen(element->name) + 3];
  char closing_tag_find[strlen(element->name) + 3];

  strcpy(closing_tag, "</");
  strcat(closing_tag, element->name);
  strcat(closing_tag, ">");

  if (s[1] == '/')
  {
    strncpy(closing_tag_find, s, strlen(element->name) + 3);
    if (strcmp(closing_tag, closing_tag_find) == 0)
    {
      return true;
    }
  }
  return false;
}

xml_element *parse_xml(char *xml)
{
  xml_element *current_element = NULL;

  while ((xml = strchr(xml, '<')) != NULL)
  {
    if (!check_is_balise(&xml))
    {
      continue;
    }
    printf("%s\n\n", xml);
    current_element = get_next_element(xml);

    if (is_closing_tag(xml, current_element))
    {
      current_element = current_element->parent;
    }
    else
    {
      // current_element = get_next_element(xml);
    }
  }
  return current_element;
}