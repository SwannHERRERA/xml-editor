#include "parse_dtd.h"
#include "parse_xml.h"

bool check_xml_correspond_to_xml(XMLElement *dtd, xml_element *root);

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
  parse_dtd(dtd, root_name);

  printf("\n######## Starting PARSE XML ########\n");

  fseek(file, 0L, SEEK_SET);
  char *xml = file_get_content(file);
  xml_element *root = parse_xml(xml);
  print_element(root);
  printf("\n######## Finished PARSE XML ########\n");

  if (check_xml_correspond_to_xml(dtd, root))
  {
    printf("XML is corresponding to DTD\n");
  }
  else
  {
    printf("XML is NOT corresponding to DTD\n");
  }

  free_DTD(dtd);
  free(xml);
  fclose(file);
  free(dtd);
  free(root_name);
  return EXIT_SUCCESS;
}

bool check_xml_correspond_to_xml(XMLElement *dtd, xml_element *root)
{
  int i, j;
  XMLElement *dtd_element;
  if (strcmp(dtd->name, root->name) != 0)
  {
    return false;
  }
  for (i = 0; i < dtd->childsCount; i += 1)
  {
    dtd_element = dtd->childs[i];
    for (j = 0; j < root->childs_count; j += 1)
    {
      if (strcmp(dtd_element->name, root->childs[j]->name) != 0)
      {
        return false;
      }
    }
    // Faire un system de conteur pour que les element qui sont la et qui ne devrait pas puisse être pris en compte
    // exemple avec student
    // check les attributes
  }
  return true;
}
