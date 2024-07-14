#ifndef HELPER_H
# define HELPER_H

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct t_header {
    char        *name;
    int         selected;
    char        *filter;
} s_header;

# define MAX_SIZE 256

void    getRowFilterDefinitions(s_header columns[], const char rowFilterDefinitions[]);

int     assertIsSelectedHeader( const char header[], const char selectedColumns[] );

int     assertFilterAllows( const char cell[], char const filter[]);

#endif
