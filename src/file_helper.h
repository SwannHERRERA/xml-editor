#ifndef _file_helper_h_
#define _file_helper_h_

#include <stdlib.h>
#include <stdio.h>

long get_size_of_file(FILE *file);
char *file_get_content(FILE *file);

#endif