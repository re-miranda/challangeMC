#include "helper.h"

void    getRowFilterDefinitions(s_header Columns[], const char rowFilterDefinitions[]) {
    char    *searchResult;
    char    *rowFilterDefinitionsCopy;
    char    *cell;
    int     found_flag;

    FILE    *outputLine;
    size_t  outputLineLen;
    char    *previus_value;


    if (rowFilterDefinitions == NULL)
        return ;
    rowFilterDefinitionsCopy = strdup(rowFilterDefinitions);
    if (rowFilterDefinitionsCopy == NULL)
        return ;
    cell  = strtok(rowFilterDefinitionsCopy, "\n");
    while (cell) {
        found_flag = 0;
        for ( s_header *inColumn = Columns; inColumn->name != NULL; ++inColumn ) {
            searchResult = strstr(cell, inColumn->name);
            if (searchResult != NULL \
                && strchr("=><", *(searchResult + strlen(inColumn->name))) != NULL) {
                previus_value = inColumn->filter;
                inColumn->filter = NULL;
                outputLineLen = 0;
                outputLine = open_memstream(&inColumn->filter, &outputLineLen);
                if (previus_value) {
                    fputs(previus_value, outputLine);
                    free(previus_value);
                }
                fprintf(outputLine, "%s,", cell + strlen(inColumn->name));
                fclose(outputLine);
                found_flag = 1;
                break ;
            }
        }
        if (found_flag == 0) {
            write(2, &"Invalid filter: ", 16);
            write(2, cell, strlen(cell));
            write(2, &"\n", 1);

        }
        cell  = strtok(NULL, "\n");
    }
    free(rowFilterDefinitionsCopy);
    return ;
}

int assertIsSelectedHeader( const char header[], const char selectedColumns[] ) {
    char    *searchResult;

    if (selectedColumns == NULL)
        return (0);
    searchResult = strstr(selectedColumns, header);
    if (searchResult == NULL)
        return (0);
    if (searchResult > selectedColumns)
        if (*(searchResult - 1) != ',')
            return (0);
    if (strlen(searchResult) > strlen(header))
        if (*(searchResult + strlen(header)) != ',')
            return (0);
    return (1);
}

int assertFilterAllows( const char cell[], char const filter[]) {
    char        *filterCopy;
    char        *filterContextPtr;
    char        *singleFilter;
    int         cmpResult;
    int         allow_flag;

    allow_flag = 1;
    if (filter == NULL)
        return (1);
    filterCopy = strdup(filter);
    if (filterCopy == NULL)
        return (1);
    filterContextPtr = filterCopy;
    singleFilter  = strtok_r(filterCopy, ",", &filterContextPtr);
    (void)cell;
    while (singleFilter) {
        cmpResult = strcmp(cell, singleFilter + 1);
        if (*singleFilter == '=') {
            if (cmpResult != 0)
                allow_flag = 0;
        } else if (*singleFilter == '>') {
            if (cmpResult <= 0)
                allow_flag = 0;
        } else if (*singleFilter == '<') {
            if (cmpResult >= 0)
                allow_flag = 0;
        }
        singleFilter = strtok_r(NULL, ",", &filterContextPtr);
    }
    free(filterCopy);
    return (allow_flag);
}
