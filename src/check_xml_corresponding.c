#include "check_xml_corresponding.h"

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
    bool is_not_in_dtd;
    int tab[dtd_element->childsCount];
    bool error = false;

    for (i = 0; i < dtd_element->childsCount; i += 1)
    {
        tab[i] = 0;
    }

    for (i = 0; i < element->childs_count; i += 1)
    {
        is_not_in_dtd = true;
        for (j = 0; j < dtd_element->childsCount; j += 1)
        {
            if (strcmp(element->childs[i]->name, dtd_element->childs[j]->name) == 0)
            {
                is_not_in_dtd = false;
                tab[j] += 1;
                if (check_element_is_correct(dtd_element->childs[j], element->childs[i]) == false)
                {
                    return false;
                }
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

    for (i = 0; i < dtd_element->childsCount; i += 1)
    {
        // printf("%s %d(%c) tab[%d]: %d\n", dtd_element->childs[i]->name, dtd_element->childs[i]->occurenceFlag, dtd_element->childs[i]->occurenceChar, i, tab[i]);
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