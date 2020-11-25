#include "check_xml_corresponding.h"

int menu()
{
  int input = 0;
  do
  {
    printf("Quel fichier voulez vous ouvrir ?\n");
    printf("Option 1: basic.xml\n");
    printf("Option 2: basic-error.xml\n");
    printf("Option 3: basic-attribute.xml\n");
    printf("Option 4: school.xml\n");
    printf("Option 5: profondeur-multiple.xml\n");
    scanf("%d", &input);
  } while (input <= 0 || input > 5);
  return input;
}

int main(void)
{
  const char *files_names[] = {
      "./xml-test/basic.xml",
      "./xml-test/basic-error.xml",
      "./xml-test/basic-attribute.xml",
      "./xml-test/school.xml",
      "./xml-test/profondeur-multiple.xml"};

  int user_entry = menu();
  user_entry -= 1;

  FILE *file = fopen(files_names[user_entry], "r");
  if (file == NULL)
  {
    fprintf(stderr, "Error opening file %s\n", files_names[user_entry]);
    return EXIT_FAILURE;
  }
  char *xml_file_content = file_get_content(file);
  char *root_name = NULL;
  char *dtd_string = find_doctype(xml_file_content, &root_name);
  DTD_element *dtd = parse_dtd(dtd_string, root_name);

  printf("\n######## Starting PARSE XML ########\n");
  fseek(file, 0L, SEEK_SET);
  XML_element *root = parse_xml(xml_file_content);
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

  if (dtd != NULL)
  {
    free_DTD(dtd);
  }
  free_element(root);
  free(xml_file_content);
  fclose(file);
  free(dtd_string);
  free(root_name);
  return EXIT_SUCCESS;
}
