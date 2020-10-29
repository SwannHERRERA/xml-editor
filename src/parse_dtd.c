#include "parse_dtd.h"

// TODO attention http
char *find_doctype(FILE *file, char **root_name)
{
  char *buffer = file_get_content(file);
  char *start = strstr(buffer, "<!DOCTYPE ");
  char size_of_doctype = get_size_of_doctype(start);
  char *doctype = (char *)malloc(sizeof(char) * size_of_doctype+1);
  if (!doctype)
  {
    fprintf(stderr, "Failed to allocate memory for {doctype} [find_doctype]");
    exit(EXIT_FAILURE);
  }
  strncpy(doctype, start, size_of_doctype);
  *root_name = get_root_name(doctype);
  doctype[(int)size_of_doctype] = 0;
  free(buffer);
  if (is_internal_doctype(doctype))
  {
    char * str = get_between_tokens(doctype, "[]");
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

/*TODO check why start is equal to nil
 * Gets every char between two tokens 
 * tokens[0] is where the new string will begin
 * tokens[1] is the token where the new string will stop
 */ 
char *get_between_tokens(char *buffer, char *tokens)
{
  int size = 0;
  char *start = NULL;
  char *end = NULL;
  char *buff = NULL;
  start = strchr(buffer, tokens[0])+1;
  end = strchr(buffer, tokens[1])-1;
  size = end - start + 2;
  if(start == NULL || end == NULL || end-start < 1)
  {
    fprintf(stderr, "Invalid DTD, it maybe empty %s %p %p\n",tokens, start, end);
    return NULL;
  }
  buff = malloc(sizeof(char) * size);
  strncpy(buff, start, size);
  buff[size-1] = 0;
  return buff;
}

bool is_xml_valid_char(char c)
{
  return ((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z') || (c >= '0' && c <= '9')) ? true : false;
}

char *get_root_name(char* buffer)
{
  char *start = strstr(buffer, "<!DOCTYPE ") + strlen("<!DOCTYPE ");
  int char_count = 0, j = 0;
  bool found = false;
  while (is_xml_valid_char(*(start + j)) || !found)
  {
    if (is_xml_valid_char(*(start + j)) && !found)
    {
      found = true;
      start = start + j;
      j=0;
    }
    if (is_xml_valid_char(*(start + j)))
    {
      char_count++;
    }
    j++;
  }
  char *root_name = (char*) malloc(sizeof(char) * char_count+1);
    if (root_name == NULL)
  {
    fprintf(stderr, "Memory allocation failed [get_root_name]");
    exit(EXIT_FAILURE);
  }
  strncpy(root_name, start, char_count);
  root_name[char_count] = 0;
  return root_name;
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
  bool no_delim = (strchr(dtd, delim) == NULL ? true : false);
  *size = char_count(dtd, delim);
  if(no_delim)
  {
    *size = 1;
  }
  char **buffer = malloc(sizeof(char *) * (*size)+1);
  if (buffer == NULL)
  {
    fprintf(stderr, "Failed to allocate memory [parse_dtd]\n");
    exit(EXIT_FAILURE);
  }
  if(no_delim)
  {
    buffer[0] = dtd;
    return buffer;
  }
  int i = 0;
  buffer[i] = strtok(dtd, &delim);
  while (buffer[i] != NULL)
  {
    i+=1;
    buffer[i] = strtok(NULL, &delim);
  }
  return buffer;
}

char *get_node_name(char *buffer)
{
  char *ptr_str = strstr(buffer, "<!ELEMENT ");
  int j = (ptr_str - buffer) + strlen("<!ELEMENT ");
  bool found = false;
  char *name_start = NULL;
  int name_length = 0;
  while (is_xml_valid_char(buffer[j]) || !found)
  {
    if (is_xml_valid_char(buffer[j]) && !found)
    {
      found = true;
      name_start = buffer + j;
    }
    if (is_xml_valid_char(buffer[j]))
    {
      name_length++;
    }
    j++;
  }
  char *name = malloc(sizeof(char) * name_length);
  strncpy(name, name_start, name_length);
  name[name_length] = 0;
  return name;
}


XMLElement *parse_element(char *node_name, char **buffer, int buffer_size)
{
  XMLElement *xml_element = NULL;
  for (int i = 0; i < buffer_size; i++)
  {
    char *ptr_str = strstr(buffer[i], "<!ELEMENT ");
    if (ptr_str != NULL)
    {
     char *name = get_node_name(buffer[i]);
     if(strcmp(node_name, name) == 0)
     {
      ptr_str = strstr(buffer[i], name) + strlen(name);
      xml_element = create_element(name);
      bool found_any = false;
      bool found_empty = false;
      char *elements = NULL;
      int elements_size = 1;
      char **elements_buffer = NULL;

      if(strstr(ptr_str, "ANY") != NULL)
      {
        found_any = true;
      }
      if(strstr(ptr_str, "EMPTY") != NULL)
      {
        found_empty = true;
      }
      elements = get_between_tokens(ptr_str, "()");
      if((found_any && found_empty) || (found_any && elements != NULL) || (found_empty && elements != NULL))
      {
        fprintf(stderr, "Error at : %s>\n", buffer[i]);
        exit(EXIT_FAILURE);
      }
      elements_buffer = split_string(elements, &elements_size, ',');
      for(int j = 0; j < elements_size; j += 1)
      {
        if(strstr(elements_buffer[j], "#PCDATA") != NULL)
        {
          continue;
        }
        int k = 0;
        int name_length = 0;
        bool found =false;
        char *name_start = NULL;
        char *element_name = NULL;
        while(is_xml_valid_char(*(elements_buffer[j] + k)) || !found)
        {
          if (is_xml_valid_char(*(elements_buffer[j] + k)) && !found)
          {
            found = true;
            name_start = elements_buffer[j] + k;
          }
          if (is_xml_valid_char(*(elements_buffer[j] + k)))
          {
            name_length++;
          }
          k++;
        }
        if(name_length > 0){
          element_name = malloc(sizeof(char) * name_length + 1);
          if (buffer == NULL)
          {
            fprintf(stderr, "Failed to allocate memory [name] {parse_element}\n");
            exit(EXIT_FAILURE);
          }
          strncpy(element_name, name_start, name_length);
          element_name[name_length] = 0;
          add_element(xml_element, parse_element(element_name, buffer, buffer_size));
        }
      }
      free(elements);
      break;
     }
    }
  }
  return xml_element;
}

XMLElement *parse_dtd(char *dtd, char *root_name)
{
  printf("Starting to parse dtd\n");
  printf("DTD : %s\nRoot name : %s\n", dtd, root_name);
  int buffer_size = 0;
  char **buffer = split_string(dtd, &buffer_size, '>');
  XMLElement *parent = parse_element(root_name, buffer, buffer_size);
  printf("child = %s\n", parent->childs[0]->name);
  free(buffer);
  return parent;
}