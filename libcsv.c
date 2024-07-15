#include "libcsv.h"
#include "helper.h"

void processCsv( const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *tmpFile;

    tmpFile = fopen("./tmp", "w+");
    if (tmpFile == NULL)
        return ;
    fputs(csvData, tmpFile);
    fseek(tmpFile, 0, SEEK_SET);
    processCsvFile("./tmp", selectedColumns, rowFilterDefinitions);
    fclose(tmpFile);
    remove("./tmp");
    return ;
}

void processCsvFile( const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *stream;
    char    *line;
    size_t  len;
    ssize_t readBytes;
    s_header    columns[MAX_SIZE];
    size_t      columnsSize;

    stream = fopen(csvFilePath, "r");
    if (stream == NULL) {
        return ;
    }
    line = NULL;
    len = 0;
    memset(columns, 0, sizeof(columns));
    readBytes = getline(&line, &len, stream);
    if (readBytes > 0) {
        columnsSize = processCsvColumns(line, columns, selectedColumns);
        if (columnsSize > 0 && parseInputs(columns, selectedColumns, rowFilterDefinitions)) {
            getRowFilterDefinitions(columns, rowFilterDefinitions);
            readBytes = getline(&line, &len, stream);
            outputColumns(columns, columnsSize);
            while (readBytes > 0) {
                processCsvLine(line, columns);
                readBytes = getline(&line, &len, stream);
            }
        }
        freeColumns(columns, columnsSize);
    }
    if (line)
        free(line);
    fclose(stream);
    return ;
}

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

    if (!csvLine)
        return ;
    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return ;
    contextPtr = csvLineCopy;
    outputLine = open_memstream(&outputLinePtr, &outputLineLen);
    if (!outputLine)
        return ;
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
