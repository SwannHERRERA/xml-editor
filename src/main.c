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

  char *xml = "<classrooms attr='fefe' b='ef' c='>' derrerzer='qs'>"
              "< classroom > AL</ classroom>"
              "<classroom> IABD</ classroom>"
              "<classroom> MOC</ classroom>"
              "<classroom> IBC</ classroom>"
              "</classrooms> ";

  get_element(xml, root_name);

  parse_dtd(dtd, root_name);
  fclose(file);
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
