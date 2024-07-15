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
    char    *token;
    size_t  headerIndex;
    int     firstRunFlag;
    int     discartFlag;

    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return ;

    outputLine = open_memstream(&outputLinePtr, &outputLineLen);
    if (!outputLine) {
        free (csvLineCopy);
        return ;
    }

    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    contextPtr = csvLineCopy;
    token  = strtok_r(csvLineCopy, ",", &contextPtr);
    headerIndex = 0;
    firstRunFlag = 1;
    discartFlag = 0;
    while (token) {
        if (columns[headerIndex].selected == 1) {
            if (!assertFilterAllows(token, columns[headerIndex].filter) )
                discartFlag = 1;
            if (!firstRunFlag)
                fputs(",", outputLine);
            firstRunFlag = 0;
            fputs(token, outputLine);
        }
        token  = strtok_r(NULL, ",", &contextPtr);
        ++headerIndex;
    }
    fclose(outputLine);
    if (!discartFlag)
        if (outputLinePtr[0] != 0)
            printf("%s\n", outputLinePtr);
    free(outputLinePtr);
    free(csvLineCopy);
    return ;
}

size_t    processCsvColumns(const char csvLine[], s_header columns[], const char selectedColumns[]) {
    char    *csvLineCopy;
    char    *contextPtr;
    char    *token;
    size_t  index;

    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return (-1);

    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    contextPtr = csvLineCopy;
    token  = strtok_r(csvLineCopy, ",", &contextPtr);
    index = 0;
    while (token) {
        if (index >= MAX_SIZE)
            break ;
        columns[index].name = strdup(token);
        columns[index].selected = assertIsSelectedHeader(token, selectedColumns);
        columns[index].filter = NULL;
        index++;
        token  = strtok_r(NULL, ",", &contextPtr);
    }
    free(csvLineCopy);
    if (index >= MAX_SIZE)
        return (-3);
    return (index);
}

void    getRowFilterDefinitions(s_header Columns[], const char rowFilterDefinitions[]) {
    char    *searchResult;
    char    *rowFilterDefinitionsCopy;
    char    *contextPtr;
    char    *token;
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
    token = strtok_r(rowFilterDefinitionsCopy, "\n", &contextPtr);
    while (token) {
        foundFlag = 0;
        for (s_header *inColumn = Columns; inColumn->name != NULL; ++inColumn ) {
            searchResult = strstr(token, inColumn->name);
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
                fprintf(outputLine, "%s,", token + strlen(inColumn->name));
                fclose(outputLine);
                foundFlag = 1;
                break ;
            }
        }
        if (foundFlag == 0) {
            write(2, &"Invalid filter: ", 16);
            write(2, token, strlen(token));
            write(2, &"\n", 1);
        }
        token = strtok_r(NULL, "\n", &contextPtr);
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

int assertFilterAllows( const char token[], char const filter[]) {
    char        *filterCopy;
    char        *contextPtr;
    char        *singleFilter;
    int         cmpResult;
    int         cmpResult2;
    int         allowFlag;

    if (filter == NULL)
        return (1);

    filterCopy = strdup(filter);
    if (filterCopy == NULL)
        return (1);

    allowFlag = 1;
    contextPtr = filterCopy;
    singleFilter  = strtok_r(filterCopy, ",", &contextPtr);
    while (singleFilter) {
        cmpResult = strcmp(token, singleFilter + 1);
        cmpResult2 = strcmp(token, singleFilter + 2);
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
