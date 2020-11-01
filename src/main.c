#include "parse_dtd.h"

// Faire une liste chainé
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

xml_element *get_root_balise(char *xml, char *root_name);

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
  char *dtd = find_doctype(file, &root_name);
  fclose(file);

  char *xml = "<classrooms attr='fefe' b='ef' c='>' derrerzer='qs'>"
              "< classroom > AL</ classroom>"
              "<classroom> IABD</ classroom>"
              "<classroom> MOC</ classroom>"
              "<classroom> IBC</ classroom>"
              "</classrooms> ";

  get_root_balise(xml, root_name);

  parse_dtd(dtd, root_name);
  free(dtd);
  free(root_name);
  return EXIT_SUCCESS;
}

bool isWhiteSpaceCharacter(char c)
{
  unsigned int size = 8;
  unsigned int i;
  char whiteSpaceCharacter[] = {32, 9, 10, 11, 12, 13, 133, 160};
  for (i = 0; i < size; i += 1)
  {
    if (whiteSpaceCharacter[i] == c)
    {
      return true;
    }
  }
  return false;
}

xml_attribute_linkedlist *create_next_element(xml_attribute_linkedlist *head)
{
  xml_attribute_linkedlist *new_attribute = malloc(sizeof(xml_attribute_linkedlist));
  new_attribute->next = NULL;
  new_attribute->value = NULL;
  head->next = new_attribute;
  return new_attribute;
}

void make_attributes(char *tag, char *subject, xml_element *element)
{
  char *start;
  size_t i = 0, j = 0, counter;
  char *tmp = (char *)calloc(strlen(tag) + 1, sizeof(char));
  strcpy(tmp, "<");
  start = strstr(subject, strcat(tmp, tag));
  tmp = start;

  xml_attribute_linkedlist *head = element->attributes;

  // Care about <b attr=">">
  while (tmp[i] != '>')
  {
    if (tmp[i] == '=')
    {
      element->number_of_attribute += 1;
      xml_attribute *attr = malloc(sizeof(xml_attribute));
      j = i;
      counter = 0;
      while (tmp[j] != ' ')
      {
        j -= 1;
        counter += 1;
      }
      attr->name = calloc(counter, sizeof(char));
      strncpy(attr->name, tmp + i - counter, counter);

      i += 2; // skip ="
      j = i;
      while (tmp[j] != '"' && tmp[j] != '\'' && j < strlen(tmp))
      {
        j += 1;
      }
      attr->value = calloc(j - i, sizeof(char));
      strncpy(attr->value, tmp + i, j - i);

      element->attributes->value = attr;
      element->attributes = create_next_element(element->attributes);
    }

    if (i > strlen(start))
    {
      fprintf(stderr, "No '>' in xml \n");
      exit(EXIT_FAILURE);
    }
    i += 1;
  }
  element->attributes = head;
}

void create_empty_xml_attribute_linkedlist(xml_element *element)
{
  element->attributes = malloc(sizeof(xml_attribute_linkedlist));
  element->attributes->value = NULL;
  element->attributes->next = NULL;
}

void print_attribute(xml_element *element)
{
  unsigned int i = 0;
  xml_attribute_linkedlist *head = element->attributes;
  printf("number of attributes: %d\n", element->number_of_attribute);
  for (i = 0; i < element->number_of_attribute; i += 1)
  {
    printf("%s=\"%s\"\n", head->value->name, head->value->value);

    head = head->next;
  }
}

xml_element *get_root_balise(char *xml, char *root_name)
{
  xml_element *root_tag = malloc(sizeof(xml_element));
  root_tag->name = root_name;
  root_tag->number_of_attribute = 0;
  create_empty_xml_attribute_linkedlist(root_tag);
  make_attributes(root_name, xml, root_tag);
  print_attribute(root_tag);

  return root_tag;
}