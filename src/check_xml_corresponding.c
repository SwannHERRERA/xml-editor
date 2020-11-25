#include "check_xml_corresponding.h"

bool check_dtd_correspond_to_xml(DTD_element *dtd, XML_element *root)
{
    if (dtd != NULL)
    {
        if (strcmp(dtd->name, root->name) == 0)
        {
            return check_element_is_correct(dtd, root);
        }
    }
    return false;
}

bool check_element_is_correct(DTD_element *dtd_element, XML_element *element)
{
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
                if (!check_element_is_correct(dtd_element->childs[j], element->childs[i]) || check_error_attributes(dtd_element->childs[j], element->childs[i]))
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

    for (i = 0; i < dtd_element->childsCount; i += 1)
    {
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

bool check_error_attributes(DTD_element *dtd_element, XML_element *element)
{
    unsigned int i, j;
    bool attribute_exist;
    bool error = false;
    xml_attribute **attributes = attributes_to_array(element);
    DTD_attribute **dtd_attributes = attributes_dtd_to_array(dtd_element);
    int max_size;
    if (element->number_of_attribute >  dtd_element->numberOfAttribute) {
        max_size = element->number_of_attribute;
    } else {
        max_size = dtd_element->numberOfAttribute;
    }
    int elements_in_dtd[max_size];
    for (i = 0; i < max_size; i += 1)
    {
        elements_in_dtd[i] = 0;
    }
    for (j = 0; j < element->number_of_attribute; j += 1)
    {
        attribute_exist = false;
        for (i = 0; i < dtd_element->numberOfAttribute; i += 1)
        {
            printf("attrbute:%s len:%ld dtd:%s len:%ld\n",
                   attributes[j]->name,
                   strlen(attributes[j]->name),
                   dtd_attributes[i]->name,
                   strlen(dtd_attributes[i]->name));
            if (strcmp(dtd_attributes[i]->name, attributes[j]->name) == 0)
            {
                elements_in_dtd[i] = 1;
                attribute_exist = true;
            }
        }
        if (!attribute_exist)
        {
            error = true;
            fprintf(stderr, "%s is not in dtd\n", attributes[j]->name);
        }
    }

    for (i = 0; i < dtd_element->numberOfAttribute; i += 1)
    {
        printf("%s %d\n",dtd_attributes[i]->name,dtd_attributes[i]->value);
        switch (dtd_attributes[i]->value)
        {
        case REQUIRED:
            if (elements_in_dtd[i] < 1)
            {
                error = true;
                fprintf(stderr, "error %s\n", dtd_attributes[i]->name);
            }
            break;
        case IMPLIED:
            break;
        case FIXED:
            break;
        default:
            break;
        }
    }
    printf("\n");
    free(attributes);
    free(dtd_attributes);
    return error;
}