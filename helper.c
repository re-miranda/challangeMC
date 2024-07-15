#include "helper.h"

void    getRowFilterDefinitions(s_header Columns[], const char rowFilterDefinitions[]) {
    char    *searchResult;
    char    *rowFilterDefinitionsCopy;
    char    *contextPtr;
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
    contextPtr = rowFilterDefinitionsCopy;
    cell = strtok_r(rowFilterDefinitionsCopy, "\n", &contextPtr);
    while (cell) {
        foundFlag = 0;
        for (s_header *inColumn = Columns; inColumn->name != NULL; ++inColumn ) {
            searchResult = strstr(cell, inColumn->name);
            if (searchResult != NULL \
                && assertFilterOperatorIsValid(searchResult + strlen(inColumn->name))) {
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
        cell = strtok_r(NULL, "\n", &contextPtr);
    }
    if (rowFilterDefinitionsCopy)
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
    char        *contextPtr;
    char        *singleFilter;
    int         cmpResult;
    int         cmpResult2;
    int         allowFlag;

    allowFlag = 1;
    if (filter == NULL)
        return (1);
    filterCopy = strdup(filter);
    if (filterCopy == NULL)
        return (1);
    contextPtr = filterCopy;
    singleFilter  = strtok_r(filterCopy, ",", &contextPtr);
    (void)cell;
    while (singleFilter) {
        cmpResult = strcmp(cell, singleFilter + 1);
        cmpResult2 = strcmp(cell, singleFilter + 2);
        if (singleFilter[0] == '=') {
            if (cmpResult != 0)
                allowFlag = 0;
        } else if (singleFilter[0] == '!' && singleFilter[1] == '=') {
            if (cmpResult2 == 0)
                allowFlag = 0;
        } else if (singleFilter[0] == '>' && singleFilter[1] == '=') {
            if (cmpResult2 < 0)
                allowFlag = 0;
        } else if (singleFilter[0] == '>') {
            if (cmpResult <= 0)
                allowFlag = 0;
        } else if (singleFilter[0] == '<' && singleFilter[1] == '=') {
            if (cmpResult2 > 0)
                allowFlag = 0;
        } else if (singleFilter[0] == '<') {
            if (cmpResult >= 0)
                allowFlag = 0;
        }
        singleFilter = strtok_r(NULL, ",", &contextPtr);
    }
    if (filterCopy)
        free(filterCopy);
    return (allowFlag);
}

void    outputColumns(s_header columns[], size_t columnsSize) {
    int         firstOutputFlag;

    firstOutputFlag = 1;
    for (size_t in = 0; in < columnsSize; ++in) {
        if (columns[in].selected == 1) {
            if (firstOutputFlag != 1)
                printf(",");
            printf("%s", columns[in].name);
            firstOutputFlag = 0;
        }
    }
    if (firstOutputFlag != 1)
        printf("\n");
    return ;
}

void    freeColumns(s_header columns[], size_t columnsSize) {
    for (size_t in = 0; in < columnsSize; ++in) {
        if (columns[in].name)
            free(columns[in].name);
        if (columns[in].filter)
            free(columns[in].filter);
    }
    return ;
}

int assertFilterOperatorIsValid(char filterTail[]) {
    if (strchr("=><", *filterTail) == NULL)
        return (1);
    else if (strncmp("!=", filterTail, 2) != 0)
        return (1);
    return (0);
}
