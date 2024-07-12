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
size_t  processCsvColumns(const char csvLine[], s_header columns[]);

void processCsv( const char csvData[], const char selectedColumns[], const char rowFilterDefinitions[] ) {
    FILE    *tmp_file;

    tmp_file = fopen("./tmp", "w+");
    if ( tmp_file == NULL )
        return ;
    fputs(csvData, tmp_file);
    fseek(tmp_file, 0, SEEK_SET);
    processCsvFile("./tmp", selectedColumns, rowFilterDefinitions);
    remove("./tmp");
    fclose(tmp_file);
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
        if (columns_size < 1) {
            if (line)
                free(line);
            line = NULL;
            return ;
        }
        readBytes = getline(&line, &len, stream);
    }
    while (readBytes > 0) {
        processCsvLine(line);
        readBytes = getline(&line, &len, stream);
    }
    if (line)
        free(line);
    for (size_t in = 0; in < columns_size; ++in) {
        free(columns[in].name);
    }
    fclose(stream);
    // (void)csvFilePath;
    (void)selectedColumns;
    (void)rowFilterDefinitions;
    return ;
}

void    processCsvLine(const char csvLine[]) {
    char    *csvLineCopy;
    FILE    *outputLine;
    char    *outputLinePtr;
    size_t  outputLineLen;
    char    *cell;

    if (!csvLine)
        return ;
    csvLineCopy = strdup(csvLine);
    if (!csvLineCopy)
        return ;
    outputLine = open_memstream(&outputLinePtr, &outputLineLen);
    if (!outputLine)
        return ;
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
    cell  = strtok(csvLineCopy, ",");
    while (cell) {
        fputs(cell, outputLine);
        fputs(" ", outputLine);
        cell  = strtok(NULL, ",");
    }
    fclose(outputLine);
    free(csvLineCopy);
    if (!cell)
        write(1, outputLinePtr, strlen(outputLinePtr));
    free(outputLinePtr);
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
    if (!csvLineCopy)
        return (-1);
    csvLineCopy[strcspn(csvLineCopy, "\n")] = 0;
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
    free(csvLineCopy);
    return (index);
}
