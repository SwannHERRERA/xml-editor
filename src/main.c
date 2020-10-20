#include "parse_dtd.h"

bool isCorrectXML(FILE *xml);

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
  if (!isCorrectXML(file))
  {
    fprintf(stderr, "Error XML invalid");
  }
  char *dtd = find_doctype(file);
  fclose(file);
  parse_dtd(dtd);
  free(dtd);
  return EXIT_SUCCESS;
}

bool isCorrectXML(FILE *xml)
{
  // Check 1 chevron ouvrant = 1 chevron fermant ⚠️ commentaire
  // Check nombre pair de " non \\
  // Il faut pas qu'il y ait de "<! "

  return true;
}