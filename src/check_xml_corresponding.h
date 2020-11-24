#ifndef _check_xml_corresponding_h_
#define _check_xml_corresponding_h_

#include "parse_dtd.h"
#include "parse_xml.h"

bool check_dtd_correspond_to_xml(XMLDTDElement *dtd, xml_element *root);
bool check_element_is_correct(XMLDTDElement *dtd_element, xml_element *element);
bool check_error_attributes(XMLDTDElement *dtd_element, xml_element *element);

#endif