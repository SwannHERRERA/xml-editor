#ifndef _check_xml_corresponding_h_
#define _check_xml_corresponding_h_

#include "parse_dtd.h"
#include "parse_xml.h"

bool check_dtd_correspond_to_xml(DTD_element *dtd, XML_element *root);
bool check_element_is_correct(DTD_element *dtd_element, XML_element *element);
bool check_error_attributes(DTD_element *dtd_element, XML_element *element);

#endif