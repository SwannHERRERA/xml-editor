#include "parse_dtd.h"

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
  char *dtd = find_doctype(file);
  fclose(file);
  parse_dtd(dtd);
  free(dtd);
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