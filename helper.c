#include "helper.h"

void    getRowFilterDefinitions(s_header Columns[], const char rowFilterDefinitions[]) {
    char    *searchResult;
    char    *rowFilterDefinitionsCopy;
    char    *cell;
    int     foundFlag;
    FILE    *outputLine;
    size_t  outputLineLen;
    char    *previusValue;

    if (rowFilterDefinitions == NULL)
        return ;
    rowFilterDefinitionsCopy = strdup(rowFilterDefinitions);
    if (rowFilterDefinitionsCopy == NULL)
        return ;
    cell = strtok(rowFilterDefinitionsCopy, "\n");
    while (cell) {
        foundFlag = 0;
        for (s_header *inColumn = Columns; inColumn->name != NULL; ++inColumn ) {
            searchResult = strstr(cell, inColumn->name);
            if (searchResult != NULL \
                && strchr("=><", *(searchResult + strlen(inColumn->name))) != NULL) {
                previusValue = inColumn->filter;
                inColumn->filter = NULL;
                outputLineLen = 0;
                outputLine = open_memstream(&inColumn->filter, &outputLineLen);
                if (previusValue) {
                    fputs(previusValue, outputLine);
                    free(previusValue);
                }
                fprintf(outputLine, "%s,", cell + strlen(inColumn->name));
                fclose(outputLine);
                foundFlag = 1;
                break ;
            }
        }
        if (foundFlag == 0) {
            write(2, &"Invalid filter: ", 16);
            write(2, cell, strlen(cell));
            write(2, &"\n", 1);
        }
        cell = strtok(NULL, "\n");
    }
    free(rowFilterDefinitionsCopy);
    return ;
}

int assertIsSelectedHeader( const char header[], const char selectedColumns[] ) {
    char    *searchResult;

    if (selectedColumns == NULL || selectedColumns[0] == 0)
        return (1);
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
    int         allowFlag;

    allowFlag = 1;
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
                allowFlag = 0;
        } else if (*singleFilter == '>') {
            if (cmpResult <= 0)
                allowFlag = 0;
        } else if (*singleFilter == '<') {
            if (cmpResult >= 0)
                allowFlag = 0;
        }
        singleFilter = strtok_r(NULL, ",", &filterContextPtr);
    }
    free(filterCopy);
    return (allowFlag);
}
