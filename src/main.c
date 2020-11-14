#include "parse_dtd.h"
#include "parse_xml.h"

bool check_xml_correspond_to_xml(XMLElement *dtd, xml_element *root);
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
  free(dtd_string);
  free(root_name);
  return EXIT_SUCCESS;
}

bool check_xml_correspond_to_xml(XMLElement *dtd, xml_element *root)
{
  if (strcmp(dtd->name, root->name) == 0)
  {
    return check_element_is_correct(dtd, root);
  }
  return false;
}
bool check_element_is_correct(XMLElement *dtd_element, xml_element *element)
{
  int i, j;
  /**
   * Cas a géré un élément qui ne trouve pas sa pair 
   * Pour la dtd
   * Faire un count nombre d'élément trouvé += pour chaque nombre d'élément trouvé diférent
   * et verifié que ce count = childCount 
   * 
   * Gestion des occurences
   * 
   * Faire un tableau des possiblités ["classroom", "student"]
   * si + alors je lis le flag est ce que j'ai trouvé un element de ce type
   * si rien je delete l'élément du tableau
   * si * je le laisse dans le tableau
   * si ? je le retire mais si je le croise a la fin j'ignore
   */
  for (i = 0; i < element->childs_count; i += 1)
  {
    // flag j'ai trouvé = false
    for (j = 0; j < dtd_element->childsCount; j += 1)
    {
      printf("name: %s occurenceChar: %c\n", dtd_element->childs[j]->name, dtd_element->childs[j]->occurenceChar);
      if (strcmp(element->childs[i]->name, dtd_element->childs[j]->name) == 0)
      {
        if (check_element_is_correct(dtd_element->childs[j], element->childs[i]) == false)
        {
          return false;
        }
        // flag j'ai trouvé = true
      }
    }
  }
  // check les attributes
  return true;
}