#include "parse_dtd.h"

// TODO attention http
char *find_doctype(FILE *file, char **root_name)
{
  char *buffer = file_get_content(file);
  char *start = strstr(buffer, "<!DOCTYPE ");
  long size_of_doctype = get_size_of_doctype(start);
  char *doctype = (char *)malloc(sizeof(char) * size_of_doctype + 1);
  if (!doctype)
  {
    fprintf(stderr, "Failed to allocate memory for {doctype} [find_doctype]");
    exit(EXIT_FAILURE);
  }
  strncpy(doctype, start, size_of_doctype);
  size_t cursor = strlen("<!DOCTYPE ");
  *root_name = get_next_name(strstr(buffer, "<!DOCTYPE "), &cursor);
  doctype[(int)size_of_doctype] = 0;
  free(buffer);
  if (is_internal_doctype(doctype))
  {
    char *str = get_between_tokens(doctype, &cursor, "[]");
    return str;
  }
  else
  {
    return get_content_of_external_DTD(doctype);
  }
}

char *get_content_of_external_DTD(char *doctype)
{
  char *res = NULL;
  char *external_DTD_filename = NULL;
  FILE *external_DTD_file = NULL;

  external_DTD_filename = get_DTD_filename(doctype);
  external_DTD_file = fopen(external_DTD_filename, "r");

  if (external_DTD_file == NULL)
  {
    fprintf(stderr, "Failed to open file: %s\n", external_DTD_filename);
    fclose(external_DTD_file);
    exit(EXIT_FAILURE);
  }

  res = file_get_content(external_DTD_file);
  fprintf(stderr, "file content %s\n", res);
  free(external_DTD_filename);
  free(doctype);
  fclose(external_DTD_file);
  return res;
}

char *get_DTD_filename(char *doctype)
{
  char *res;
  long size_of_doctype = strlen(doctype);
  int n = 0;
  int start = 0;
  int end = 0;
  while (n < size_of_doctype)
  {
    if (doctype[n] == '"')
    {
      if (start == 0)
      {
        start = n + 1;
      }
      else
      {
        end = n;
        break;
      }
    }
    n += 1;
  }
  res = (char *)malloc(sizeof(char) * (end - start));
  if (res == NULL)
  {
    fprintf(stderr, "Memory allocation failed [get_DTD_filename]");
    exit(EXIT_FAILURE);
  }
  strncpy(res, doctype + start, end - start);
  return res;
}

long get_size_of_doctype(char *start)
{
  long size_of_buffer = strlen(start);
  long n = 1;
  int nb_open_tags = 0;
  int exit_loop = 0;

  while (n < size_of_buffer && exit_loop == 0)
  {
    if (start[n] == '<')
    {
      nb_open_tags += 1;
    }
    else if (start[n] == '>')
    {
      if (nb_open_tags == 0)
      {
        exit_loop = 1;
      }
      else
      {
        nb_open_tags -= 1;
      }
    }
    n += 1;
  }
  return n;
}

bool is_internal_doctype(char *doctype)
{
  if (strstr(doctype, "SYSTEM") == NULL)
  {
    return true;
  }
  return false;
}

/* Gets every char between two tokens 
 * tokens[0] is where the new string will begin
 * tokens[1] is the token where the new string will stop
 */
char *get_between_tokens(char *buffer, size_t *cursor, char *tokens)
{
  int size = 0;
  char *start = NULL;
  char *end = NULL;
  char *buff = NULL;
  start = strchr(buffer + (*cursor), tokens[0]) + 1;
  end = strchr(buffer, tokens[1]) - 1;
  size = end - start + 1;
  if (buffer == NULL || start == NULL || end == NULL || end - start < 1)
  {
    fprintf(stderr, "Invalid DTD, it maybe empty %s\n", tokens);
    return NULL;
  }
  printf("aaaaaaa  %s %s\n", start, buffer);
  buff = malloc(sizeof(char) * size + 1);
  strncpy(buff, start, size);
  buff[size] = 0;
  *cursor += size + 3;
  return buff;
}

bool is_xml_valid_char(char c)
{
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) ? true : false;
}

char *get_next_name(char *ptr_str, size_t *offset)
{
  bool found = false;
  char *name_start = NULL;
  int name_length = 0;
  char *name = NULL;
  while (is_xml_valid_char(*(ptr_str + (*offset))) || !found)
  {
    if (is_xml_valid_char(*(ptr_str + (*offset))) && !found)
    {
      found = true;
      name_start = ptr_str + (*offset);
    }
    if (is_xml_valid_char(*(ptr_str + (*offset))))
    {
      name_length++;
    }
    (*offset)++;
  }
  if (name_length > 0)
  {
    name = (char *)malloc(sizeof(char) * name_length + 1);
  }
  if (name == NULL)
  {
    fprintf(stderr, "Failed to allocate memory [name] {parse_element}\n");
    exit(EXIT_FAILURE);
  }
  strncpy(name, name_start, name_length);
  name[name_length] = 0;
  return name;
}

int char_count(char *str, char character)
{
  int counter = 0;
  for (size_t i = 0; i < strlen(str); i++)
  {
    if (str[i] == character)
    {
      counter++;
    }
  }
  return counter;
}

char **split_string(char *dtd, int *size, char delim)
{
  char **buffer = NULL;
  bool no_delim = (strchr(dtd, delim) == NULL ? true : false);
  *size = 0;
  if (no_delim)
  {
    buffer = malloc(sizeof(char *) * (*size) + 1);

    buffer[0] = malloc(sizeof(char) * strlen(dtd) + 1);
    if (buffer == NULL)
    {
      fprintf(stderr, "Failed to allocate memory [parse_dtd]\n");
      exit(EXIT_FAILURE);
    }
    strcpy(buffer[0], dtd);
    buffer[0][strlen(dtd)] = 0;
    *size = 1;
    return buffer;
  }
  buffer = malloc(sizeof(char *) * (*size) + 1);
  char *str_new = strtok(dtd, &delim);
  if (str_new != NULL)
  {
    buffer[*size] = malloc(sizeof(char) * strlen(str_new) + 1);
    strcpy(buffer[*size], str_new);
    buffer[*size][strlen(str_new)] = 0;
  }
  while (buffer[*size] != NULL)
  {
    char *str = strtok(NULL, &delim);
    *size += 1;
    char **new_buffer = malloc(sizeof(char *) * (*size) + 1);
    for (int i = 0; i < *size; i++)
    {
      new_buffer[i] = buffer[i];
    }
    if (str != NULL)
    {
      new_buffer[*size] = malloc(sizeof(char) * strlen(str) + 1);
      strcpy(new_buffer[*size], str);
      new_buffer[*size][strlen(str)] = 0;
    }
    free(buffer);
    buffer = new_buffer;
  }
  for (int i = 0; i < *size; i++)
  {
    printf("aaaaaaaaaaaaaaaa %s\n", buffer[i]);
  }
  return buffer;
}

void set_global_child_occurence(char global_occurence_char, XMLElement *parent)
{
  for (int i = 0; i < parent->childsCount; i++)
  {
    set_child_occurence(global_occurence_char, parent->childs[i]);
  }
}

void set_child_occurence(char occurence_char, XMLElement *child)
{
  const char c[] = {'?', '+', '*', '|'};
  const size_t c_size = 4;
  int pos = -1;
  for (size_t i = 0; i < c_size; i++)
  {
    if (occurence_char == c[i])
    {
      pos = i;
    }
  }
  switch (pos)
  {
  case 0:
    child->occurenceFlag |= OCCURENCE_0_1;
    break;
  case 1:
    child->occurenceFlag |= OCCURENCE_1_N;
    break;
  case 2:
    child->occurenceFlag |= OCCURENCE_0_N;
    break;
  case 3:
    child->occurenceFlag |= OCCURENCE_OR;
    break;
  default:
    break;
  }
}

void parse_element_childs(XMLElement *parent, int elements_size, char **elements_buffer, char **buffer, int buffer_size)
{
  for (int i = 0; i < elements_size; i += 1)
  {
    if (strstr(elements_buffer[i], "#PCDATA") != NULL)
    {
      continue;
    }
    size_t cursor = 0;
    char *element_name = get_next_name(elements_buffer[i], &cursor);
    if (element_name != NULL)
    {
      XMLElement *child = parse_sub_element(element_name, buffer, buffer_size);
      if (child != NULL)
      {
        if (elements_buffer[i][cursor] != 0)
        {
          printf("occurence char = %c \n", elements_buffer[i][cursor]);
          child->occurenceChar = elements_buffer[i][cursor];
          set_child_occurence(elements_buffer[i][cursor], child);
        }
        add_element(parent, child);
      }
    }
  }
}

char *get_node_childs(char *buffer, char *name, char *last_char)
{
  char *ptr_str = strstr(buffer, name);
  size_t cursor = strlen(name);
  bool found_any = false;
  bool found_empty = false;
  char *elements = NULL;

  if (strstr(ptr_str, "ANY") != NULL)
  {
    found_any = true;
  }
  if (strstr(ptr_str, "EMPTY") != NULL)
  {
    found_empty = true;
  }
  elements = get_between_tokens(ptr_str, &cursor, "()");
  if (elements == NULL)
  {
    return NULL;
  }
  *last_char = *(ptr_str + cursor);
  if ((found_any && found_empty) || (found_any && elements != NULL) || (found_empty && elements != NULL))
  {
    fprintf(stderr, "Error at : %s>\n", buffer);
    exit(EXIT_FAILURE);
  }
  return elements;
}

AttributeValue get_attribute_value(char **str)
{
  const char *names[] = {"", "#REQUIRED", "#IMPLIED", "#FIXED"};
  int size = 4;
  int value = -1;
  for (int i = 0; i < size; i++)
  {
    if (!strcmp(*str, names[i]))
    {
      if (value == -1)
      {
        value = i;
      }
      else
      {
        printf("ERROR at %s\n", *str);
      }
      char *st = strstr(*str, names[i]);
      if (st != NULL && st > *str)
      {
        *str = strstr(*str, names[i]);
      }
    }
  }
  return value;
}

AttributeType get_attribute_type(char **str)
{
  const char *names[] = {"CDATA"};
  int size = 1;
  int type = -1;
  for (int i = 0; i < size; i++)
  {
    if (!strcmp(*str, names[i]))
    {
      if (type == -1)
      {
        type = i;
      }
      else
      {
        printf("ERROR at %s\n", *str);
      }
      char *st = strstr(*str, names[i]);
      if (st != NULL && st > *str)
      {
        *str = strstr(*str, names[i]);
      }
    }
  }
  return type;
}

void parse_attributes(XMLElement *element, char **buffer, int buffer_size)
{
  for (int i = 0; i < buffer_size; i++)
  {
    char *ptr_str = strstr(buffer[i], "<!ATTLIST ");
    if (ptr_str != NULL)
    {
      size_t cursor = strlen("<!ATTLIST ");
      char *node_name = get_next_name(ptr_str, &cursor);
      if (!strcmp(element->name, node_name))
      {
        char *attribute_name = get_next_name(ptr_str, &cursor);
        AttributeType type = get_attribute_type(&ptr_str);
        AttributeValue value = get_attribute_value(&ptr_str);
        add_attribute(element, attribute_name, value, type);
      }
      free(node_name);
    }
  }
}

XMLElement *parse_element(char *node_name, char **buffer, int buffer_size)
{
  XMLElement *xml_element = NULL;
  for (int i = 0; i < buffer_size; i++)
  {
    if (buffer[i] != NULL)
    {
      char *ptr_str = strstr(buffer[i], "<!ELEMENT ");
      if (ptr_str != NULL)
      {
        size_t cursor = strlen("<!ELEMENT ");
        char *name = get_next_name(ptr_str, &cursor);
        if (strcmp(node_name, name) == 0)
        {
          xml_element = complete_element(buffer, buffer_size, i, name);
          break;
        }
      }
    }
  }
  return xml_element;
}

XMLElement *parse_sub_element(char *node_name, char **buffer, int buffer_size)
{
  XMLElement *xml_element = NULL;
  for (int i = 0; i < buffer_size; i++)
  {
    char *ptr_str = strstr(buffer[i], "<!ELEMENT ");
    if (ptr_str != NULL)
    {
      size_t cursor = strlen("<!ELEMENT ");
      char *name = get_next_name(ptr_str, &cursor);
      if (strcmp(node_name, name) == 0)
      {
        xml_element = complete_element(buffer, buffer_size, i, name);
        break;
      }
    }
  }
  return xml_element;
}

XMLElement *complete_element(char **buffer, int buffer_size, int index, char *name)
{
  XMLElement *xml_element = create_element(name);
  char global_occurence_char = 0;
  char *elements = get_node_childs(buffer[index], name, &global_occurence_char);
  if (elements != NULL)
  {
    int elements_size = 1;
    char **elements_buffer = split_string(elements, &elements_size, ',');
    parse_element_childs(xml_element, elements_size, elements_buffer, buffer, buffer_size);
    set_global_child_occurence(global_occurence_char, xml_element);
    parse_attributes(xml_element, buffer, buffer_size);
    free(elements_buffer);
    free(elements);
  }
  return xml_element;
}

XMLElement *parse_dtd(char *dtd, char *root_name)
{
  printf("######## Starting to parse DTD ########\n");
  XMLElement *parent = NULL;
  if (dtd != NULL && strlen(dtd) > 0)
  {
    printf("DTD : %s\nRoot name : %s\n", dtd, root_name);
    int buffer_size = 0;
    char **buffer = split_string(dtd, &buffer_size, '>');
    parent = parse_element(root_name, buffer, buffer_size);
    set_deepness(parent);
    print_tree(parent);
    for (int i = 0; i < buffer_size; i++)
    {
      free(buffer[i]);
    }
    free(buffer);
    printf("######## Finished parsing DTD ########\n");
  }
  return parent;
}