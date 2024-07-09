#include "libcsv.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>

void    processCsvLine(const char csvLine[]);

void processCsv( const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    processCsvLine(csvData);
    (void)selectedColumns;
    (void)rowFilterDefinitions;
    return ;
}

void processCsvFile( const char csvFilePath[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    (void)csvFilePath;
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
