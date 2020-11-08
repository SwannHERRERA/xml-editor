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
xml_element *get_next_element(char *xml, xml_element *parent)
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
  element->parent = parent;
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
  xml_element *current_element = NULL;

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
        print_element(current_element);
      }
      current_element = current_element->parent;
    }
    else
    {
      current_element = get_next_element(xml, current_element);
    }
    xml = xml + sizeof(char) * 1;
  }
  return current_element;
}