#include "parse_dtd.h"
#include "parse_xml.h"
#include "interface.h"

bool check_dtd_correspond_to_xml(XMLElement *dtd, xml_element *root);
bool check_element_is_correct(XMLElement *dtd_element, xml_element *element);
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

  printf("\n######## Starting PARSE XML ########\n");

  fseek(file, 0L, SEEK_SET);
  char *xml = file_get_content(file);
  xml_element *root = parse_xml(xml);
  print_element(root);
  printf("\n######## Finished PARSE XML ########\n");

  if (check_dtd_correspond_to_xml(dtd, root))
  {
    printf("XML is corresponding to DTD\n");
  }
  else
  {
    printf("XML is NOT corresponding to DTD\n");
  }

  free_DTD(dtd);
  free_element(root);
  free(xml);
  fclose(file);
  free(dtd_string);
  free(root_name);
  // init_interface(&argc, &argv);
  return EXIT_SUCCESS;
}

bool check_dtd_correspond_to_xml(XMLElement *dtd, xml_element *root)
{
  if (strcmp(dtd->name, root->name) == 0)
  {
    return check_element_is_correct(dtd, root);
  }
  return false;
}

bool check_element_is_correct(XMLElement *dtd_element, xml_element *element)
{

  // J'ai un problème quand je trouve une balise que je n'attends pas + balise autofermante
  int i, j;
  size_t k;
  bool is_not_in_dtd;
  int tab[dtd_element->childsCount];
  bool error = false;

  for (i = 0; i < dtd_element->childsCount; i += 1)
  {
    tab[i] = 0;
  }

  printf("childs_count: %d\n", element->childs_count);
  for (i = 0; i < element->childs_count; i += 1)
  {
    is_not_in_dtd = true;
    for (j = 0; j < dtd_element->childsCount; j += 1)
    {
      for (k = 0; k < strlen(element->childs[i]->name); k += 1)
      {
        printf("%c ", element->childs[i]->name[k]);
      }
      printf("\n");
      // if (strcmp(element->childs[i]->name, "carte") == 0)
      // {
      //   printf("diff: %d\n", strcmp(element->childs[i]->name, dtd_element->childs[j]->name));
      // }
      // printf("element: %s, dtd: %s, element_size: %ld, dtd_size: %ld\n", element->childs[i]->name, dtd_element->childs[j]->name, strlen(element->childs[i]->name), strlen(dtd_element->childs[j]->name));
      if (strcmp(element->childs[i]->name, dtd_element->childs[j]->name) == 0)
      {
        is_not_in_dtd = false;
        tab[j] += 1;
        continue;
      }
    }
    if (is_not_in_dtd)
    {
      fprintf(stderr, "error element: %s is not in dtd\n", element->childs[i]->name);
      return false;
    }
  }

  // check les attributes

  // check with occurenceFlag
  for (i = 0; i < dtd_element->childsCount; i += 1)
  {
    printf("%s %d(%c) tab[%d]: %d\n", dtd_element->childs[i]->name, dtd_element->childs[i]->occurenceFlag, dtd_element->childs[i]->occurenceChar, i, tab[i]);
    switch (dtd_element->childs[i]->occurenceFlag)
    {
    case OCCURENCE_1_N:
      if (tab[i] < 1)
      {
        error = true;
        fprintf(stderr, "erreur la balise %s doit avoir des elements %s\n", dtd_element->name, dtd_element->childs[i]->name);
      }
      break;
    case OCCURENCE_0_N:
      // DO nothing
      break;
    case OCCURENCE_0_1:
      if (tab[i] > 1)
      {
        error = true;
        fprintf(stderr, "erreur la balise %s doit avoir entre 0 et 1 element %s\n", dtd_element->name, dtd_element->childs[i]->name);
      }
      break;
    case OCCURENCE_1_1:
      if (tab[i] != 1)
      {
        error = true;
        fprintf(stderr, "erreur la balise %s doit avoir 1 element %s\n", dtd_element->name, dtd_element->childs[i]->name);
      }
      break;

    default:
      break;
    }
  }
  return !error;
}