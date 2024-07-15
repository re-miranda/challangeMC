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

int     parseInputs(s_header const columns[], char const selectedColumns[], char const rowFilterDefinitions[]);

void    processCsvLine( const char csvLine[], s_header columns[]);

size_t  processCsvColumns( const char csvLine[], s_header columns[], const char selectedColumns[]);

void    getRowFilterDefinitions(s_header columns[], const char rowFilterDefinitions[]);

int     assertIsSelectedHeader( const char header[], const char selectedColumns[] );

int     assertFilterAllows( const char cell[], char const filter[]);

void    outputColumns(s_header columns[], size_t columnsSize);

void    freeColumns(s_header columns[], size_t columnsSize);

int     assertFilterOperatorIsValid(char filterTail[]);

#endif
