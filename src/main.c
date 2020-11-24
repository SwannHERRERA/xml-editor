#include "check_xml_corresponding.h"
#include "interface.h"

int main(int argc, char **argv)
{
  /*
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
  char *xml_file_content = file_get_content(file);
  char *root_name = NULL;
  char *dtd_string = find_doctype(xml_file_content, &root_name);
  XMLElement *dtd = parse_dtd(dtd_string, root_name);

  printf("\n######## Starting PARSE XML ########\n");
  printf("aaa%s\n",xml_file_content);
  fseek(file, 0L, SEEK_SET);
  xml_element *root = parse_xml(xml_file_content);
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

  if (dtd != NULL)
  {
    free_DTD(dtd);
  }
  free_element(root);
  free(xml_file_content);
  fclose(file);
  free(dtd_string);
  free(root_name);*/
  init_interface(&argc, &argv);
  return EXIT_SUCCESS;
}
