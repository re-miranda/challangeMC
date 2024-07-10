#include "libcsv.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct t_filter {
    char            operator;
    int             comparisson_value;
    struct t_filter *next_filter;
} s_filter;

typedef struct t_header {
    char        *name;
    s_filter    *filter;
} s_header;

# define MAX_SIZE 256

void    processCsvLine(const char csvLine[]);
size_t     processCsvColumns(const char csvLine[], s_header columns[]);

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

    s_header    columns[MAX_SIZE];
    size_t      columns_size;

    stream = fopen(csvFilePath, "r");
    if (stream == NULL){
        write(1, &"Failed to open file\n", 20);
        return ;
    }
    line = NULL;
    len = 0;
    readBytes = getline(&line, &len, stream);
    if (readBytes > 0) {
        columns_size = processCsvColumns(line, columns);
        if (columns_size < 1)
            return ;
        readBytes = getline(&line, &len, stream);
    }
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

size_t    processCsvColumns(const char csvLine[], s_header columns[]) {
    char  *csvLineCopy;
    char  *cell;
    size_t  index;

    if (!csvLine)
        return (-1);
    csvLineCopy = strdup(csvLine);
    cell  = strtok(csvLineCopy, ",");
    index = 0;
    while (cell) {
        if (index >= MAX_SIZE)
            return (-2);
        columns[index].name = strdup(cell);
        columns[index].filter = NULL;
        index++;
        cell  = strtok(NULL, ",");
    }
    return (index);
}
