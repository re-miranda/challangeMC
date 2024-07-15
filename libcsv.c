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
