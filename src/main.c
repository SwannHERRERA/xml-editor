#include "parse_dtd.h"
#include "parse_xml.h"

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

  fseek(file, 0L, SEEK_SET);
  char *xml = file_get_content(file);
  get_element(xml, root_name);

  free(xml);
  fclose(file);
  free(dtd);
  free(root_name);
  return EXIT_SUCCESS;
}