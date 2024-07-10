#include "libcsv.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void    processCsvLine(const char csvLine[]);

void processCsv( const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    processCsvLine(csvData);
    (void)selectedColumns;
    (void)rowFilterDefinitions;
    return ;
}

void processCsvFile( const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *stream;
    char    *line;
    size_t  len;
    ssize_t readBytes;

    stream = fopen(csvFilePath, "r");
    if (stream == NULL){
        write(1, &"Failed to open file\n", 20);
        return ;
    }
    line = NULL;
    len = 0;
    readBytes = getline(&line, &len, stream);
    while (readBytes > 0) {
        processCsvLine(line);
        if (line)
            free(line);
        line = NULL;
        readBytes = getline(&line, &len, stream);
    }
    // (void)csvFilePath;
    (void)selectedColumns;
    (void)rowFilterDefinitions;
    return ;
}

void    processCsvLine(const char csvLine[]) {
    char  *csvLineCopy;
    char  *cell;

    if (!csvLine)
        return ;
    csvLineCopy = strdup(csvLine);
    cell  = strtok(csvLineCopy, ",");
    while (cell) {
        write(1, cell, strlen(cell));
        write(1, &" | ", 3);
        cell  = strtok(NULL, ",");
    }
    write(1, &"\n", 1);
    return ;
}
