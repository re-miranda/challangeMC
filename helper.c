#include "helper.h"

int parseInputs(s_header const columns[], char const selectedColumns[], char const rowFilterDefinitions[]) {
    char    *copy;
    char    *contextPtr;
    char    *token;
    int     abortFlag;

    abortFlag = 0;
    if (selectedColumns != NULL) {
        copy = strdup(selectedColumns);
        if (copy == NULL)
            return (0);
        contextPtr = copy;
        token = strtok_r(copy, ",", &contextPtr);
        while (token && abortFlag == 0) {
            abortFlag = 1;
            for (s_header *inColumn = (s_header *)columns; inColumn->name != NULL; ++inColumn ) {
                if (strcmp(token, inColumn->name) == 0 \
                    && 1) {
                    abortFlag = 0;
                    break ;
                }
            }
            if (abortFlag == 1) {
                write(2, &"Header '", 8);
                write(2, token, strlen(token));
                write(2, &"' not found in CSV file/string\n", 32);
            }
            token = strtok_r(NULL, ",", &contextPtr);
        }
        if (copy)
            free(copy);
    }
    if (rowFilterDefinitions != NULL && abortFlag == 0) {
        copy = strdup(rowFilterDefinitions);
        if (copy == NULL)
            return (0);
        contextPtr = copy;
        token = strtok_r(copy, "\n", &contextPtr);
        while (token && abortFlag == 0) {
            abortFlag = 1;
            for (s_header *inColumn = (s_header *)columns; inColumn->name != NULL; ++inColumn ) {
                if (strstr(token, inColumn->name) == token \
                    && strchr("=!<>", token[strlen(inColumn->name)]) != NULL) {
                    abortFlag = 0;
                    break ;
                }
            }
            if (abortFlag == 1) {
                write(2, &"Header '", 8);
                write(2, token, strlen(token));
                write(2, &"' not found in CSV file/string\n", 32);
            }
            token = strtok_r(NULL, "\n", &contextPtr);
        }
        if (copy)
            free(copy);
    }
    return (!abortFlag);
}

void    processCsvLine( const char csvLine[], s_header columns[]) {
    char    *csvLineCopy;
    FILE    *outputLine;
    char    *outputLinePtr;
    char    *contextPtr;
    size_t  outputLineLen;
    char    *cell;
    size_t  headerIndex;
    int     firstRunFlag;
    int     discartFlag;

    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return ;
    contextPtr = csvLineCopy;
    outputLine = open_memstream(&outputLinePtr, &outputLineLen);
    if (!outputLine) {
        free (csvLineCopy);
        return ;
    }
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    cell  = strtok_r(csvLineCopy, ",", &contextPtr);
    headerIndex = 0;
    firstRunFlag = 1;
    discartFlag = 0;
    while (cell) {
        if (columns[headerIndex].selected == 1) {
            if (!assertFilterAllows(cell, columns[headerIndex].filter) )
                discartFlag = 1;
            if (!firstRunFlag)
                fputs(",", outputLine);
            firstRunFlag = 0;
            fputs(cell, outputLine);
        }
        cell  = strtok_r(NULL, ",", &contextPtr);
        ++headerIndex;
    }
    fclose(outputLine);
    if (!discartFlag)
        if (outputLinePtr[0] != 0)
            printf("%s\n", outputLinePtr);
    if (cell)
        free(cell);
    if (outputLinePtr)
        free(outputLinePtr);
    if (csvLineCopy)
        free(csvLineCopy);
    return ;
}

size_t    processCsvColumns(const char csvLine[], s_header columns[], const char selectedColumns[]) {
    char    *csvLineCopy;
    char    *contextPtr;
    char    *cell;
    size_t  index;

    if (!csvLine)
        return (-1);
    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return (-1);
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    contextPtr = csvLineCopy;
    cell  = strtok_r(csvLineCopy, ",", &contextPtr);
    index = 0;
    while (cell) {
        if (index >= MAX_SIZE)
            break ;
        columns[index].name = strdup(cell);
        columns[index].selected = assertIsSelectedHeader(cell, selectedColumns);
        columns[index].filter = NULL;
        index++;
        cell  = strtok_r(NULL, ",", &contextPtr);
    }
    free(csvLineCopy);
    if (cell) {
        free(cell);
        return (-3);
    }
    return (index);
}

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

int assertFilterOperatorIsValid(char filterTail[]) {
    if (strchr("=><", *filterTail) == NULL)
        return (1);
    else if (strncmp("!=", filterTail, 2) != 0)
        return (1);
    return (0);
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

char    *getCsvLine(FILE *stream) {
    char        *line;
    size_t      len;
    ssize_t     readBytes;

    line = NULL;
    len = 0;
    readBytes = getline(&line, &len, stream);
    if (readBytes <= 0) {
        if (line)
            free(line);
        return (NULL);
    }
    return (line);
}
