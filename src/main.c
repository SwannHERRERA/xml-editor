#include "check_xml_corresponding.h"

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
  // print_element(root);
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
  return EXIT_SUCCESS;
}
