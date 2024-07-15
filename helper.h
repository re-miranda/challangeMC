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

/**
 * searches for invalid headers in selectedColumns or rowFilterDefinitions by tokenizing them and comparing with columns.name.
 *
 * @param s_header data pointer to be used as reference.
 * @param csv string containing selected columns definition.
 * @param new-line separated string containing filter definitions.
 *
 * @return returns 0 if invalid headers are found.
 */
int     parseInputs(s_header const columns[], char const selectedColumns[], char const rowFilterDefinitions[]);

void    processCsvLine( const char csvLine[], s_header columns[]);

/**
 * inserts names of each column into columns.name by tokenizing selectedColumns.
 *
 * @param csv line to be processed.
 * @param s_header data pointer that will receive name data.
 * @param csv string containing selected columns definition.
 *
 * @return void
 */
size_t  processCsvColumns( const char csvLine[], s_header columns[], const char selectedColumns[]);

/**
 * inserts filter definitions into columns.filter by searching tokenized rowFilterDefinitions and concatenating repeated results.
 *
 * @param s_header data pointer that will receive filter data.
 * @param new-line separated string containing filter definitions.
 *
 * @return void
 */
void    getRowFilterDefinitions(s_header columns[], const char rowFilterDefinitions[]);

/**
 * asserts if the content of header exists in selectedColumns by searching the selectedColumns string.
 *
 * @param string to be analysed.
 * @param csv string containing selected headers.
 *
 * @return result of the assertion
 */
int     assertIsSelectedHeader( const char header[], const char selectedColumns[] );

/**
 * asserts if the content of token is allowed by applying tokenizing the filter.
 *
 * @param string to be analysed.
 * @param new-line separated string containing filters.
 *
 * @return result of the assertion
 */
int     assertFilterAllows( const char token[], char const filter[]);

/**
 * asserts if the operators included in filter are valid by comparing the first 2 characters.
 *
 * @param string to be analysed.
 *
 * @return result of the assertion
 */
int     assertFilterOperatorIsValid(char filterTail[]);

/**
 * output columns names to stdout by iterating through columns and inserting separators.
 *
 * @param s_header data pointer to be processed.
 * @param the size of the data.
 *
 * @return void
 */
void    outputColumns(s_header columns[], size_t columnsSize);

/**
 * free allocated memmory by iterating through columns.
 *
 * @param s_header data pointer to be processed.
 * @param the size of the data.
 *
 * @return void
 */
void    freeColumns(s_header columns[], size_t columnsSize);

#endif
