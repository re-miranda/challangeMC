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
        if (columnsSize > 0) {
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

void    processCsvLine( const char csvLine[], s_header columns[]) {
    char    *csvLineCopy;
    FILE    *outputLine;
    char    *outputLinePtr;
    char    *csvLineCopyContextPtr;
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
    csvLineCopyContextPtr = csvLineCopy;
    outputLine = open_memstream(&outputLinePtr, &outputLineLen);
    if (!outputLine)
        return ;
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    cell  = strtok_r(csvLineCopy, ",", &csvLineCopyContextPtr);
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
        cell  = strtok_r(NULL, ",", &csvLineCopyContextPtr);
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
    char    *cell;
    size_t  index;

    if (!csvLine)
        return (-1);
    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return (-1);
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    cell  = strtok(csvLineCopy, ",");
    index = 0;
    while (cell) {
        if (index >= MAX_SIZE)
            break ;
        columns[index].name = strdup(cell);
        columns[index].selected = assertIsSelectedHeader(cell, selectedColumns);
        columns[index].filter = NULL;
        index++;
        cell  = strtok(NULL, ",");
    }
    free(csvLineCopy);
    if (cell) {
        free(cell);
        return (-3);
    }
    return (index);
}
